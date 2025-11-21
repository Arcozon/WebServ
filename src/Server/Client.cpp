/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 14:59:48 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/30 15:04:15 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Cookies.hpp"

const std::string Client::_supportedHTTPVersion = "HTTP/1.1";
const std::string Client::_sepLine = "\r\n";
const std::size_t Client::_sepLineLen = _sepLine.size();
const std::size_t Client::_bufferSize = 1024;

Client::Client(int fd, IpPort *config, Sessions *instance)
	: _fd(fd),
	  _config(config),
	  _request_len(0),
	  _last_pos(0),
	  _request_step(REQUEST_LINE),
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
	_response = new Response(this);
}

Client::~Client()
{
	close(_fd);
	delete (_response);
}

bool Client::_makeExtractLine(void)
{
	// _pos = _str_buffer.find(_sepLine, _last_pos);
	// if(_pos == std::string::npos)
	// 	return (false);
	// else
	// {
	// 	//std::cout << "Got full request line" << std::endl << _str_buffer.substr(_last_pos, _pos + 2) << std::endl;
	// 	_extract_line = _str_buffer.substr(_last_pos, _pos - _last_pos);
	// 	// std::cout << "Full line recieved: " << _extract_line << std::endl;
	// 	// std::cout  << "\e[34m"<< _extract_line << "\e[0m" << std::endl;
	// 	_last_pos = _pos + _sepLineLen;
	// 	return (true);
	// }

	char buffer[_bufferSize];
	int rd;

	_pos = _str_buffer.find(_sepLine, _last_pos);
	if (_pos != std::string::npos)
	{
		_extract_line = _str_buffer.substr(_last_pos, _pos - _last_pos);
		_last_pos = _pos + _sepLineLen;
		// std::cout << "line: " << _extract_line << '\n';
		return (true);
	}
	rd = read(_fd, buffer, _bufferSize);
	if (rd > 0)
	{
		updateTimer();
		_str_buffer.append(buffer, rd);
		_request_len += rd;
		_pos = _str_buffer.find(_sepLine, _last_pos);
		if (_pos != std::string::npos)
		{
			_extract_line = _str_buffer.substr(_last_pos, _pos - _last_pos);
			_last_pos = _pos + _sepLineLen;
			// std::cout << "line: " << _extract_line << '\n';
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

const std::vector<std::string> Client::_splitRequestLine(const std::string &reqLine)
{
	static const char setReqLine = ' ';
	std::vector<std::string> splitReqLine;

	if (std::count(reqLine.begin(), reqLine.end(), setReqLine) != 2)
		return (splitReqLine);

	std::size_t firstSpace = reqLine.find_first_of(setReqLine);
	std::size_t secondSpace = reqLine.find_first_of(setReqLine, firstSpace + 1);

	splitReqLine.push_back(reqLine.substr(0, firstSpace));
	splitReqLine.push_back(reqLine.substr(firstSpace + 1, secondSpace - (firstSpace + 1)));
	splitReqLine.push_back(reqLine.substr(secondSpace + 1));
	return (splitReqLine);
}

const std::pair<std::string, std::string> Client::_splitHeaderLine(const std::string &reqLine)
{
	static const std::string nameValSep = ": ";

	std::pair<std::string, std::string> splitHeaderLine;

	std::size_t indexSep = reqLine.find(nameValSep);
	// std::cout << "Index: " << indexSep << std::endl;
	if (indexSep == std::string::npos)
		return (splitHeaderLine);
	splitHeaderLine.first = reqLine.substr(0, indexSep);
	splitHeaderLine.second = reqLine.substr(indexSep + nameValSep.size());
	return (splitHeaderLine);
}

bool Client::_checkRequestLine(void) // Add IpPort (to check )
{
	std::vector<std::string> splitReqLine = _splitRequestLine(_extract_line);

	if (splitReqLine.empty())
	{
		_response->setStartLine(400);
		_response->prepare();
		_request_step = ERROR;
		return (false);
	}
	if (_extract_line.length() > MAX_HEADER_SIZE) // 8kb
	{
		_response->setStartLine(414);
		_response->prepare();
		_request_step = ERROR;
		return false;
	}
	if (splitReqLine.size() != 3)
	{
		_response->setStartLine(400);
		_response->prepare();
		_request_step = ERROR;
		return false;
	}

	_method = splitReqLine.at(0);
	_target_uri = splitReqLine.at(1);
	std::string HTTPVersion = splitReqLine.at(2);

	if (!validVerbSyntax(_method))
	{
		_response->setStartLine(400);
		_response->prepare();
		_request_step = ERROR;
		return false;
	}

	if (!(_method == "GET" || _method == "POST" || _method == "DELETE" || _method == "PUT"))
	{
		_response->setStartLine(400); // or 501
		_response->prepare();
		_request_step = ERROR;
		return (false);
	}

	if (_target_uri.length() > 1024)
	{
		_response->setStartLine(414);
		_response->prepare();
		_request_step = ERROR;
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

	if (!ValidURI(_target_uri))
	{
		_response->setStartLine(400);
		_response->prepare();
		_request_step = ERROR;
		return false;
	}

	if (_target_uri.find('\0') != std::string::npos)
	{
		_response->setStartLine(400);
		_response->prepare();
		_request_step = ERROR;
		return false;
	}

	std::cout << "\e[32m[" << _method << "]\e[33m[" << _target_uri << "]\e[34m[" << HTTPVersion << "]\e[0m" << std::endl;
	return (true);
	(void)_target_uri;
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
	if (_extract_line.empty())
	{
		std::cout << "\e[1;31m" << "	-- End Of Header --" << "\e[0m" << std::endl;
		if (!validMinimalHeaders())
		{
			_request_step = ERROR;
			return false;
		}
		std::map<std::string, std::string>::iterator it = _headers.find("Content-Length");
		if (it == _headers.end())
			it = _headers.find("content-length");

		if (it != _headers.end())
		{
			if (!validContentLength(it->second))
			{
				_request_step = ERROR;
				return false;
			}

			_content_length = std::atol(it->second.c_str());
			if (_content_length > _config->getClientMaxBodySize())
			{
				_response->setStartLine(413);
				_request_step = ERROR;
				return false;
			}
			if ((_method == "POST"))
			{
				const Location *loc = _config->getLocation(_target_uri);
				if (loc && !loc->getUploadLocation().empty())
				{
					_upload_dir = loc->getUploadLocation();
					_is_upload = true;
					std::cout << "\033[1;36m[Upload of size: " << _content_length << " bytes]\033[0m" << std::endl;
				}
				else
				{
					_response->setStartLine(403);
					_response->prepare();
					_request_step = ERROR;
					return false;
				}
			}
			else
			{
				;
			}
			_request_step = BODY;
		}
		else if (_headers.find("Transfer-Encoding") != _headers.end())
		{
			_response->setStartLine(501);
			_response->prepare();
			_request_step = ERROR;
			return false;
		}
		else
		{
			_request_step = FIN;
			std::cout << "\e[1;31m" << "	-- No Body Expected --" << "\e[0m" << std::endl;
		}
		return true;
	}
	if (_headers.size() >= 50)
	{
		_response->setStartLine(431);
		_response->prepare();
		_request_step = ERROR;
		return false;
	}

	if (_extract_line.length() > 8192)
	{
		_response->setStartLine(431);
		_response->prepare();
		_request_step = ERROR;
		return false;
	}

	_headers_total_size += _extract_line.length() + 2;
	if (_headers_total_size > HEADERS_TOTAL_MAX)
	{
		_response->setStartLine(431);
		_response->prepare();
		_request_step = ERROR;
		return false;
	}

	std::pair<std::string, std::string> _nameVal = _splitHeaderLine(_extract_line);
	if (_nameVal.first.empty())
	{
		_response->setStartLine(400);
		_response->prepare();
		_request_step = ERROR;
		return false;
	}
	if (!validHeaderSyntax(_nameVal.first))
	{
		_response->setStartLine(400);
		_response->prepare();
		_request_step = ERROR;
		return false;
	}

	size_t colon_pos = _extract_line.find(':');
	if (colon_pos != std::string::npos && colon_pos > 0)
	{
		if (std::isspace(_extract_line[colon_pos - 1]))
		{
			_response->setStartLine(400);
			_response->prepare();
			_request_step = ERROR;
			return false;
		}
	}

	if (!validDuplicateHeader(_nameVal.first))
	{
		_response->setStartLine(400);
		_response->prepare();
		_request_step = ERROR;
		return false;
	}

	if (!validHeader(_nameVal.first, _nameVal.second))
	{
		_request_step = ERROR;
		return false;
	}

	_headers.insert(_nameVal);
	if (_nameVal.first == "Cookie")
		_cookies = Cookies::parseCookie(_nameVal.second);

	std::cout << "\e[35m[" << _nameVal.first << "]\e[34m[" << _nameVal.second << "]\e[0m" << std::endl;
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
	if (name.empty())
		return false;

	for (size_t i = 0; i < name.length(); i++)
	{
		unsigned char c = name[i];
		if (std::iscntrl(c) || std::isspace(c) ||
			c == '(' || c == ')' || c == '<' || c == '>' || c == '@' ||
			c == ',' || c == ';' || c == ':' || c == '\\' || c == '"' ||
			c == '/' || c == '[' || c == ']' || c == '?' || c == '=' ||
			c == '{' || c == '}')
			return false;
	}
	return true;
}

bool Client::_checkBody(void) // IpPort + setting correct StatusLine on error
{
	char buffer[_bufferSize];
	int rd;
	size_t curr_data = _str_buffer.length() - _last_pos;
	if (curr_data > 0)
	{
		size_t left_to_read = _content_length - _body_rd_bytes;
		size_t to_copy = (curr_data < left_to_read) ? curr_data : left_to_read;

		_body_data.append(_str_buffer, _last_pos, to_copy);
		_body_rd_bytes += to_copy;
		_last_pos += to_copy;

		if (_body_rd_bytes == _content_length)
		{
			_request_step = FIN;
			std::cout << "\e[1;37m-- Received body payload (" << _body_rd_bytes << " bytes) --\e[0m" << std::endl;
			return true;
		}
	}
	rd = read(_fd, buffer, _bufferSize);
	if (rd > 0)
	{
		updateTimer();
		_str_buffer.append(buffer, rd);
		_request_len += rd;
		size_t curr_data = _str_buffer.length() - _last_pos;
		size_t left_to_read = _content_length - _body_rd_bytes;
		size_t to_copy = (curr_data < left_to_read) ? curr_data : left_to_read;

		_body_data.append(_str_buffer, _last_pos, to_copy);
		_body_rd_bytes += to_copy;
		_last_pos += to_copy;
		if (_body_rd_bytes == _content_length)
		{
			_request_step = FIN;
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

// void Client::checkStep()
// {
// 	if (!_checkCurrentLine(REQUEST_LINE))
// 	{
// 		_request_step = ERROR;
// 		return ;
// 	}
// 	std::cout  << "\e[1:31m"<< "	-- Start Of Header --" << "\e[0m" << std::endl;
// 	_request_step = HEADERS;
// 	while (_request_step == HEADERS)
// 	{
// 		if (!_checkCurrentLine(HEADERS))
// 		{
// 			_request_step = ERROR;
// 			return ;
// 		}
// 	}
// 	if (_request_step == BODY)
// 	{
// 		if (!_checkBody())
// 		{
// 			_request_step = ERROR;
// 			return ;
// 		}
// 	}
// 	if (_request_step == FIN)
// 	{
// 		std::cout << "\e[1;31m" << "	-- End Of Body --" << "\e[0m" << std::endl;
// 		if (_is_upload)
// 			fileHandler();
// 		else
// 		{
// 			_response->setBody("");
// 			_response->prepare();
// 		}
// 		_done = true;
// 		std::cout << "\033[1;34m" << "\t-- Response ready to be sent --" << "\033[0m" << std::endl;
// 	}
// }

void Client::checkStep()
{
	if (_request_step == REQUEST_LINE)
	{
		if (!_checkCurrentLine(REQUEST_LINE))
			return;
		_request_step = HEADERS;
		std::cout << "\e[1;31m\t-- Start Of Header --\e[0m" << std::endl;
	}

	// if (_request_step == HEADERS)
	// {
	// 	if (!_checkCurrentLine(HEADERS))
	// 		return ;
	// }
	if (_request_step == HEADERS)
	{
		while (_makeExtractLine())
		{
			if (!_checkHeader())
			{
				_request_step = ERROR;
				return ;
			}
			if (_request_step != HEADERS)
				break ;
		}
	}

	if (_request_step == BODY)
	{
		if (!_checkBody())
			return;
	}

	if (_request_step == FIN)
	{
		std::cout << "\e[1;31m\t-- End Of Body --\e[0m" << std::endl;

		if (_target_uri == "/session_new" && _method == "GET")
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
		else if (_target_uri == "/session_destroy" && _method == "GET")
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
		else if (_target_uri == "/session_info" && _method == "GET")
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
			fileHandler();
		else
		{
			_response->setBody("");
			_response->prepare();
		}
		_done = true;
		std::cout << "\033[1;34m\t-- Response ready to be sent --\033[0m" << std::endl;
	}
}

void Client::readFromFd()
{
	if (!_done && (_request_step != ERROR))
		checkStep();
}

void Client::sendResponse()
{
	_response->send(_fd);
}

bool Client::responseSent()
{
	return _response->isResponseFullySent();
}

bool Client::finishedReading()
{
	return _request_step == FIN;
}

void Client::fileHandler()
{
	std::string dir = _upload_dir;
	// std::string filename = _target_uri.substr(_target_uri.find_last_of('/'));
	std::string path = dir + _target_uri.substr(_target_uri.find_last_of('/'));

	std::ofstream file(path.c_str(), std::ios::binary);
	if (!file.is_open())
	{
		_response->setStartLine(500);
		_response->setBody("");
		_response->prepare();
		return;
	}
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
	if (_request_step == REQUEST_LINE || _request_step == HEADERS || _request_step == BODY)
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
	const Location *loc = _config->getLocation(_target_uri);
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
	//if(_target_uri.substr(_target_uri.find_last_of('/') + 1) != )
	std::string filename = _target_uri.substr(_target_uri.find_last_of('/') + 1);
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

// void Client::checkStep()
// {
// 	while (_request_step != FIN && _request_step != ERROR)
// 	{
// 		if(_request_step == REQUEST_LINE)
// 		{
// 			if(!checkCurrentLine())
// 				return ;
// 			else
// 				_request_step = HEADERS;
// 		}
// 		else if(_request_step == HEADERS)
// 		{
// 			if(!checkCurrentLine())
// 				return ;
// 			if(_extract_line.empty())
// 			{
// 				std::cout << "End of headers found" << std::endl;
// 				_request_step = BODY;
// 			}
// 		}
// 		else if(_request_step == BODY)
// 		{
// 			if(!checkCurrentLine())
// 				return ;
// 			else
// 			{
// 				_request_step = FIN;
// 				break;
// 			}
// 		}
// 	}
// }

// void Client::readFromFd()
// {
// 	char buffer[10];
// 	while(1)
// 	{
// 		int rd = read(_fd, buffer, 10);
// 		if(rd > 0)
// 		{
// 			_str_buffer.append(buffer, rd);
// 			_request_len += rd;
// 			checkStep();
// 			if(_request_len == ERROR)
// 				;
// 			//std::cout << "Read " << rd << " bytes: " << _str_buffer.substr(_request_len - rd) << std::endl;
// 		}
// 		else if(rd == -1)
// 			return ;
// 		else if(rd == 0)
// 		{
// 			std::cout << "Client closed connection on fd: " << _fd << std::endl;
// 			break ;
// 		}
// 	}
// 	std::cout << "Client on fd [" << _fd << "] recieved a total of " << _request_len << " bytes:" << std::endl
// 	<< _str_buffer << std::endl;
// }
