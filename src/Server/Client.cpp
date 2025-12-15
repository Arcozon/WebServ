/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 14:59:48 by gaeudes           #+#    #+#             */
/*   Updated: 2025/12/15 14:56:51 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Response.hpp"
#include "Cookies.hpp"

const std::string Client::_supportedHTTPVersion = "HTTP/1.1";
const std::string Client::_sepLine = "\r\n";
const std::size_t Client::_sepLineLen = _sepLine.size();
const std::size_t Client::_bufferSize = 1024;

Client::Client(int fd, const IpPort &config, Sessions *instance)
	: _fd(fd),
	  _config(config),
	  _request_len(0),
	  _requestStep(REQUEST_LINE),
	  _last_pos(0),
	  _done(false),
	  _response(NULL),
	  _is_upload(0),
	  _body_rd_bytes(0),
	  _last_activity(time(0)),
	  _read_timer(3),
	  _write_timer(3),
	  _client_spawn(time(0)),
	  _max_req_duration(5),
	  _session_ptr(instance),
	  _headers_total_size(0)
{
	_response = new Response(this, config);
}

Client::~Client()
{
	close(_fd);
	delete (_response);
}

bool Client::_makeExtractLine(void)
{
	char buffer[_bufferSize];
	int rd;

	_pos = _strBuffer.find(_sepLine, _last_pos);
	if (_pos != std::string::npos)
	{
		_extractedLine = _strBuffer.substr(_last_pos, _pos - _last_pos);
		_last_pos = _pos + _sepLineLen;
		return (true);
	}
	rd = read(_fd, buffer, _bufferSize);
	if (rd > 0)
	{
		updateTimer();
		_strBuffer.append(buffer, rd);
		_request_len += rd;
		_pos = _strBuffer.find(_sepLine, _last_pos);
		if (_pos != std::string::npos)
		{
			_extractedLine = _strBuffer.substr(0, _pos);
			_strBuffer.erase(0, _pos + _sepLineLen); 
			return (true);
		}
		return false;
	}
	else if (rd == 0)
	{
		_done = true;
		return (false);
	}
	else
		return (false);
}


// static inline std::size_t _countBlock(const std::string &reqLine)
inline std::size_t _countBlock(const std::string &reqLine)
{
	static const char	setReqLine = ' ';	
	std::size_t	count = 0;
	std::size_t	firstSpace;

	std::cout << "reqline[" << reqLine <<"]\n";
	for (std::size_t i = 0; i < reqLine.size(); ++count)
	{
		firstSpace = reqLine.find(setReqLine, i);
		if (firstSpace != std::string::npos)
			i = reqLine.find_first_not_of(setReqLine, firstSpace);
		else
			i = std::string::npos;
		std::cout << "count[" << count <<"]\n";
	}
	std::cout << "FINALcount[" << count <<"]\n";
	return (count); 
}

const std::vector<std::string> Client::_splitRequestLine(const std::string &reqLine)
{
	return (ParsLine::splitLine(reqLine, ' ', false));
}

const std::pair<std::string, std::string> Client::_splitHeaderLine(const std::string &reqLine)
{
	static const std::string nameValSep = ": ";

	std::pair<std::string, std::string> splitHeaderLine;

	std::size_t	indexSep = reqLine.find(nameValSep);
	if (indexSep == std::string::npos)
		return (splitHeaderLine);
	splitHeaderLine.first = reqLine.substr(0, indexSep);
	splitHeaderLine.second = reqLine.substr(indexSep + nameValSep.size());
	return (splitHeaderLine);
}

bool Client::_checkRequestLine(void) // Add IpPort (to check )
{
	std::vector<std::string>	splitReqLine = _splitRequestLine(_extractedLine);

	// std::cout << "Sise: " << splitReqLine.size() << std::endl;
	if (_extractedLine.length() > MAX_HEADER_SIZE) // 8kb
	{
		_response->setStartLine(414);
		_response->prepare();
		_requestStep = ERROR;
		return false;
	}
	if (splitReqLine.size() != 3)
	{
		_response->setStartLine(400);
		_response->prepare();
		_requestStep = ERROR;
		return false;
	}

	_method = splitReqLine.at(0);
	_requestTarget = splitReqLine.at(1);
	std::string HTTPVersion = splitReqLine.at(2);
	// std::cout << _extractedLine << '\n';
	std::cout << _method << "|"<< _requestTarget << "|"<< _HTTPVersion << '\n'; // TODO
	std::size_t	firstQuery = _requestTarget.find_first_of('?');
	if (firstQuery != std::string::npos)
	{
		_queryString = _requestTarget.substr(firstQuery + 1);
		std::cout << "Query: " << _queryString << std::endl;
		_requestTarget.resize(firstQuery);
	}
	if (!validVerbSyntax(_method))
	{
		_response->setStartLine(400);
		_response->prepare();
		_requestStep = ERROR;
		return false;
	}

	if (!(_method == "GET" || _method == "POST" || _method == "DELETE"))
	{
		_response->setStartLine(405);
		_response->prepare();
		_requestStep = ERROR;
		return (false);
	}

	if (_requestTarget.length() > 1024)
	{
		_response->setStartLine(414);
		_response->prepare();
		_requestStep = ERROR;
		return false;
	}

	// Check if method is in IpPort / location
	// check if request target is in IoPort / if [Root]/[RequestTarget] is a dir
	if (HTTPVersion != _supportedHTTPVersion)
	{
		if (HTTPVersion.substr(0, 5) == "HTTP/")
			_response->setStartLine(505);
		else
			_response->setStartLine(400);
		_response->prepare();
		return (false);
	}

	if (!ValidURI(_requestTarget))
	{
		_response->setStartLine(400);
		_response->prepare();
		_requestStep = ERROR;
		return false;
	}

	if (_requestTarget.find('\0') != std::string::npos)
	{
		_response->setStartLine(400);
		_response->prepare();
		_requestStep = ERROR;
		return false;
	}

	std::cout << "\e[32m[" << _method << "]\e[33m[" << _requestTarget << "]\e[34m[" << HTTPVersion << "]\e[0m" << std::endl;
	return (true);
}

bool Client::validVerbSyntax(const std::string &method)
{
	if (method.empty())
		return false;

	for (size_t i = 0; i < method.length(); i++)
	{
		if (!std::isupper(static_cast<unsigned char>(method[i])))
			return false;
	}
	return true;
}

bool Client::ValidURI(const std::string &uri)
{
	if (uri.empty())
		return false;

	if (uri[0] != '/')
		return false;

	for (size_t i = 0; i < uri.length(); i++)
	{
		unsigned char c = uri[i];

		if (c <= ' ' || c == 127)
			return false;
	}
	return true;
}

bool Client::_checkHeader(void)
{
	if (_extractedLine.empty())
	{
		std::cout << "\e[1;31m" << "	-- End Of Header --" << "\e[0m" << std::endl;
		if (!validMinimalHeaders())
		{
			_requestStep = ERROR;
			return false;
		}
		std::map<std::string, std::string>::iterator it = _headers.find("Content-Length");
		if (it == _headers.end())
			it = _headers.find("content-length");

		if (it != _headers.end())
		{
			if (!validContentLength(it->second))
			{
				_requestStep = ERROR;
				return false;
			}

			_content_length = std::atol(it->second.c_str());
			if (_content_length > _config.getClientMaxBodySize())
			{
				_response->setStartLine(413);
				_requestStep = ERROR;
				return false;
			}
			bool isCGI = false;
			if (_config.getLocation(_requestTarget))
				isCGI = _config.getLocation(_requestTarget)->_hasCGIHandler(this->getTargetLocation());
			if (_method == "POST"&& !isCGI)
			{
				const Location *loc = _config.getLocation(_requestTarget);
				if (loc && loc->isUploadDefined())
				{
					std::cout << "ka" << std::endl;
					_upload_dir = loc->getUploadLocation();
					_is_upload = true;
					std::cout << "\033[1;36m[Upload of size: " << _content_length << " bytes]\033[0m" << std::endl;
				}
				else
				{
					_response->setStartLine(403);
					_response->prepare();
					_requestStep = ERROR;
					_done = true;
					return false;
				}
			}
			else
			{
				;
			}
			_requestStep = BODY;
		}
		else if (_headers.find("Transfer-Encoding") != _headers.end())
		{
			_response->setStartLine(501);
			_response->prepare();
			_requestStep = ERROR;
			return false;
		}
		else
		{
			_requestStep = FIN;
			std::cout << "\e[1;31m" << "	-- No Body Expected --" << "\e[0m" << std::endl;
		}
		return (true);
	}

	if (_headers.size() >= 50)
	{
		_response->setStartLine(431);
		_response->prepare();
		_requestStep = ERROR;
		return false;
	}

	if (_extractedLine.length() > 8192)
	{
		_response->setStartLine(431);
		_response->prepare();
		_requestStep = ERROR;
		return false;
	}

	_headers_total_size += _extractedLine.length() + 2;
	if (_headers_total_size > HEADERS_TOTAL_MAX)
	{
		_response->setStartLine(431);
		_response->prepare();
		_requestStep = ERROR;
		return false;
	}

	std::pair<std::string, std::string> _nameVal = _splitHeaderLine(_extractedLine);
	if (_nameVal.first.empty())
	{
		_response->setStartLine(400);
		_response->prepare();
		_requestStep = ERROR;
		return false;
	}
	if (!validHeaderSyntax(_nameVal.first))
	{
		_response->setStartLine(400);
		_response->prepare();
		_requestStep = ERROR;
		return false;
	}

	size_t colon_pos = _extractedLine.find(':');
	if (colon_pos != std::string::npos && colon_pos > 0)
	{
		if (std::isspace(_extractedLine[colon_pos - 1]))
		{
			_response->setStartLine(400);
			_response->prepare();
			_requestStep = ERROR;
			return false;
		}
	}

	if (!validDuplicateHeader(_nameVal.first))
	{
		_response->setStartLine(400);
		_response->prepare();
		_requestStep = ERROR;
		return false;
	}

	if (!validHeader(_nameVal.first, _nameVal.second))
	{
		_requestStep = ERROR;
		return false;
	}

	_headers.insert(_nameVal);
	if (_nameVal.first == "Cookie")
		_cookies = Cookies::parseCookie(_nameVal.second);

	// std::cout << "\e[35m[" << _nameVal.first << "]\e[34m[" << _nameVal.second << "]\e[0m" << std::endl;
	return true;
}

bool Client::validHeader(const std::string &name, const std::string &value)
{
	if (name == "Content-Length")
		return validContentLength(value);
	else if (name == "Host")
	{
		if (value.empty())
		{
			_response->setStartLine(400);
			_response->prepare();
			return false;
		}
		return true;
	}
	else if (name == "Transfer-Encoding")
	{
		if (value.find("chunked") != std::string::npos)
		{
			_response->setStartLine(501);
			_response->prepare();
			return false;
		}
	}
	return true;
}

bool Client::validContentLength(const std::string &value)
{
	if (value.empty())
	{
		_response->setStartLine(400);
		_response->prepare();
		return false;
	}
	for (size_t i = 0; i < value.length(); i++)
	{
		if (!std::isdigit(value[i]))
		{
			_response->setStartLine(400);
			_response->prepare();
			return false;
		}
	}
	if (value.length() > 1 && value[0] == '0')
	{
		_response->setStartLine(400);
		_response->prepare();
		return false;
	}
	return true;
}

bool Client::validDuplicateHeader(const std::string &name)
{
	if (_headers.find(name) != _headers.end())
	{
		if (name == "Content-Length" ||
			name == "Content-Type" ||
			name == "Host" ||
			name == "Transfer-Encoding")
			return false;
	}
	return true;
}

bool Client::validMinimalHeaders()
{
	if (_headers.find("Host") == _headers.end())
	{
		_response->setStartLine(400);
		_response->prepare();
		return false;
	}
	if (_method == "POST" || _method == "PUT")
	{
		if (_headers.find("Content-Length") == _headers.end())
		{
			_response->setStartLine(411);
			_response->prepare();
			return false;
		}
	}
	return true;
}

// allowed: !#$%&'*+-.0-9A-Z^_`a-z|~
bool Client::validHeaderSyntax(const std::string &name)
{
	static const std::string	forbidenChar("()<>@,;:\\\"/[]?={}");
	if (name.empty())
		return false;

	for (size_t i = 0; i < name.length(); i++)
	{
		unsigned char c = name[i];
		if (std::iscntrl(c) || std::isspace(c) ||
			forbidenChar.find(c) != std::string::npos)
			return false;
	}
	return true;
}

bool Client::_checkBody(void) // IpPort + setting correct StatusLine on error
{
	char buffer[_bufferSize];
	int rd;
	size_t curr_data = _strBuffer.length() - _last_pos;
	if (curr_data > 0)
	{
		size_t left_to_read = _content_length - _body_rd_bytes;
		size_t to_copy = (curr_data < left_to_read) ? curr_data : left_to_read;

		_body_data.append(_strBuffer, _last_pos, to_copy);
		_body_rd_bytes += to_copy;
		_last_pos += to_copy;

		if (_body_rd_bytes == _content_length)
		{
			_requestStep = FIN;
			std::cout << "\e[1;37m-- Received body payload (" << _body_rd_bytes << " bytes) --\e[0m" << std::endl;
			return true;
		}
	}
	rd = read(_fd, buffer, _bufferSize);
	if (rd > 0)
	{
		updateTimer();
		_strBuffer.append(buffer, rd);
		_request_len += rd;
		size_t curr_data = _strBuffer.length() - _last_pos;
		size_t left_to_read = _content_length - _body_rd_bytes;
		size_t to_copy = (curr_data < left_to_read) ? curr_data : left_to_read;

		_body_data.append(_strBuffer, _last_pos, to_copy);
		_body_rd_bytes += to_copy;
		_last_pos += to_copy;
		if (_body_rd_bytes == _content_length)
		{
			_requestStep = FIN;
			std::cout << "\e[1;37m-- Received body payload (" << _body_rd_bytes << " bytes) --\e[0m" << std::endl;
			return true;
		}
		return true;
	}
	else if (rd == 0)
	{
		std::cout << "\e[1;37m-- Client closed connection while body data was being processed --\e[0m" << std::endl;
		_done = true;
		return false;
	}
	else
		return false;
}

bool Client::_checkCurrentLine(const Client::REQUEST_STEP &reqSection)
{
	if (!_makeExtractLine())
		return (false);

	switch (reqSection)
	{
		case (REQUEST_LINE):
			return (_checkRequestLine());
		case (HEADERS):
			return (_checkHeader());
		case (BODY):
			return (_checkBody());
		default:
			return (false);
	}
}

void Client::checkStep()
{
	if (_requestStep == REQUEST_LINE)
	{
		if (!_checkCurrentLine(REQUEST_LINE))
			return;
		_requestStep = HEADERS;
		std::cout << "\e[1;31m\t-- Start Of Header --\e[0m" << std::endl;
	}

	if (_requestStep == HEADERS)
	{
		while (_makeExtractLine())
		{
			if (!_checkHeader())
			{
				_requestStep = ERROR;
				return ;
			}
			if (_requestStep != HEADERS)
				break ;
		}
	}

	if (_requestStep == BODY)
	{
		if (!_checkBody())
			return;
	}

	if (_requestStep == FIN)
	{
		std::cout << "\e[1;31m\t-- End Of Body --\e[0m" << std::endl;

		if (_requestTarget == "/session_new" && _method == "GET")
		{
			std::map<std::string, std::string>::iterator it = _cookies.find("session_id");

			if (it == _cookies.end())
			{
				std::string session_id = _session_ptr->createSession();

				_session_ptr->set(session_id, "id", "gaeudes");
				Cookies session_cookie("session_id", session_id);
				session_cookie.setDuration(1200);
				_response->addCookie(session_cookie);

				_response->setStartLine(200);
				std::string res = "Session:" + session_id + " created\n";
				_response->setBody(res);
				_response->prepare();
			}
			else
			{
				_response->setStartLine(403);
				std::string res = "Session:" + it->second + " already set\n";
				_response->setBody(res);
				_response->prepare();
			}
		}
		else if (_requestTarget == "/session_destroy" && _method == "GET")
		{
			std::map<std::string, std::string>::iterator it = _cookies.find("session_id");
			if (it != _cookies.end())
			{
				std::string session_id = it->second;
				_session_ptr->erase(session_id);

				Cookies cookie("session_id", "");
				cookie.setDuration(0);
				_response->addCookie(cookie);
				_response->setStartLine(200);
				_response->setBody("Session destroyed\n");
				_response->prepare();
			}
			else
			{
				_response->setStartLine(403);
				_response->setBody("No cookie present to be destroyed\n");
				_response->prepare();
			}
		}
		else if (_requestTarget == "/session_info" && _method == "GET")
		{
			std::map<std::string, std::string>::iterator it = _cookies.find("session_id");
			if (it == _cookies.end())
			{
				_response->setStartLine(403);
				_response->setBody("No cookie set, unauthorized\n");
			}
			else
			{
				std::string session_id = it->second;

				if (!_session_ptr->exists(session_id))
				{
					_response->setStartLine(403);
					_response->setBody("Session id not present on server\n");
				}
				else
				{
					std::string id = _session_ptr->get(session_id, "id");

					if (id == "gaeudes")
					{
						_response->setStartLine(200);
						std::string body = "Le goat " + id + "\n";
						_response->setBody(body);
					}
					else
					{
						_response->setStartLine(403);
						_response->setBody("Nope");
					}
				}
			}
			_response->prepare();
		}
		if(_method == "PUT")
		{
			std::cout << "\033[1;34m\t-- PUT HANDLER CALLED --\033[0m" << std::endl;
			putHandler();
		}
		if (_is_upload)
		{
			fileHandler();
			std::cout << "Upload is done" << std::endl;
		}
		else
		{
			_response->setLocation();
			_response->prepare();
		}

		_done = true;
		std::cout << "\033[1;34m\t-- Response ready to be sent --\033[0m" << std::endl;
	}
}

void Client::readFromFd()
{

	// if (!_done && (_requestStep != ERROR))
	// 	checkStep();

	while (!_done && (_requestStep != ERROR))
	{
		REQUEST_STEP prev_step = _requestStep;
		checkStep();
		if (prev_step == _requestStep)
			break;
	}

	// if (_requestStep == ERROR)
	// {
	// 	_response->prepare();
	// 	return;
	// }
}

void Client::sendResponse()
{
	_response->send(_fd);
}

bool Client::responseSent()	const
{
	return (_response->isResponseFullySent());
}

bool Client::finishedReading()	const
{
	return (_requestStep == FIN || _requestStep == ERROR);
}

void Client::fileHandler()
{
	
	const Location *loc = _config.getLocation(_requestTarget);
	std::string strLoc = loc->getUploadLocation();
	std::string fname = _requestTarget.substr(loc->getLocation().size());
	std::string dir = loc->getUploadLocation();
	std::string path = dir + fname;

	// std::cout <<"Salut" << std::endl;
	if (!Location::isLocationPathValid(path))
	{
		_response->setStartLine(403);
		_response->setBody("");
		_response->prepare();
		return;
	}
	// std::cout <<"wsh" << std::endl;
	std::ofstream file(path.c_str(), std::ios::binary);
	if (!file.is_open())
	{
		_response->setStartLine(500);
		_response->setBody("");
		_response->prepare();
		return;
	}
	// std::cout <<"bonjour" << std::endl;
	file.write(_body_data.c_str(), _body_data.size());
	file.close();

	std::cout << "\033[1;32mFile " << path << " created\033[0m\n";
	_response->setStartLine(201);
	_response->setBody("");
	_response->prepare();
}

void Client::updateTimer()
{
	_last_activity = time(0);
}

bool Client::checkTimers()
{
	time_t diff = time(0) - _last_activity;
	time_t elapsed = time(0) - _client_spawn;
	if (_requestStep == REQUEST_LINE || _requestStep == HEADERS || _requestStep == BODY)
	{
		if (diff > _read_timer)
		{
			std::cout << "\033[1;31mClient timed out reading(+" << diff << "second(s))\033[0m" << std::endl;
			return true;
		}
		else if (elapsed > _max_req_duration)
		{
			std::cout << "\033[1;31mClient timed out reading(+" << elapsed << "second(s))\033[0m" << std::endl;
			return true;
		}
	}
	else if (_done && !_response->isResponseFullySent())
	{
		if (diff > _write_timer)
		{
			std::cout << "\033[1;31m[Client timed out writing (+" << diff << "second(s))]\033[0m" << std::endl;
			return true;
		}
	}
	return false;
}

bool Client::timedOut()
{
	return checkTimers();
}

void Client::putHandler()
{
	const Location *loc = _config.getLocation(_requestTarget);
	if (!loc)
	{
		_response->setStartLine(404);
		_response->prepare();
		return ;
	}
	std::string upload_dir = loc->getUploadLocation();
	if (upload_dir.empty())
	{
		_response->setStartLine(403);
		_response->prepare();
		return ;
	}
	//if(_requestTarget.substr(_requestTarget.find_last_of('/') + 1) != )
	std::string filename = _requestTarget.substr(_requestTarget.find_last_of('/') + 1);
	std::cout << "\033[1;32m FILENAME: " << filename << "\033[0m" << std::endl;
	if (filename.empty())
	{
		_response->setStartLine(400);
		_response->prepare();
		return ;
	}
	std::string full_dir = upload_dir;
	if (full_dir[full_dir.length() - 1] != '/')
		full_dir += '/';
	full_dir += filename;

	std::ifstream check_file(full_dir.c_str());
	bool prev_in_use = check_file.good();
	check_file.close();

	std::ofstream file(full_dir.c_str(), std::ios::binary | std::ios::trunc);
	if (!file.is_open())
	{
		_response->setStartLine(500);
		_response->prepare();
		return ;
	}
	file.write(_body_data.c_str(), _body_data.size());
	file.close();

	if (prev_in_use)
		_response->setStartLine(204);
	else
		_response->setStartLine(201);
	std::cout << "\033[1;32m PUT: File " << full_dir << " " << (prev_in_use ? "updated" : "created") << "\033[0m" << std::endl;
	_response->prepare();
}

const IpPort	&Client::getConfig(void) const
{
	return (_config);
}


const std::string	&Client::getMethod(void) const
{
	return (_method);
}

const std::string	&Client::getTargetLocation(void) const
{
	return (_requestTarget);
}

const std::string	&Client::getQueryString(void) const
{
	return (_queryString);
}

const std::map<std::string, std::string>	&Client::getHeader(void)const
{
	return (_headers);
}

const std::string	&Client::getBody(void) const
{
	return (_body_data);
}
