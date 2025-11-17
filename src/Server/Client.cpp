/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 14:59:48 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/17 14:52:06 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Response.hpp"

const std::string	Client::_supportedHTTPVersion = "HTTP/1.1";
const std::string	Client::_sepLine = "\r\n";
const std::size_t	Client::_sepLineLen = _sepLine.size();
const std::size_t	Client::_bufferSize = 1024;

Client::Client(int fd, const IpPort &config)
:	_fd(fd),
	_config(config),
	_request_len(0),
	_requestStep(REQUEST_LINE),
	_location(NULL),
	_response(NULL)
{
	// _response = new Response(this, config);
}

Client::~Client()
{
	close(_fd);
	delete(_response);
	(void)_location;
}

bool	Client::_makeExtractLine(void)
{
	char	buffer[_bufferSize];
	int		rd;

	do
	{
		_pos = _strBuffer.find(_sepLine);
		if (_pos != std::string::npos)
		{
			_extractedLine = _strBuffer.substr(0, _pos);
			_strBuffer.erase(0, _pos + _sepLineLen); 
			return (true);
		}
		rd = read(_fd, buffer, _bufferSize);
		if(rd > 0)
		{
			_strBuffer.append(buffer, rd);
			_request_len += rd;
		}
	}	while (rd > 0);
	_requestStep = FIN;
	return (true);
}

const std::vector<std::string>	Client::_splitRequestLine(const std::string &reqLine)
{
	static const char	setReqLine = ' ';	
	std::vector<std::string>	splitReqLine;

	if (std::count(reqLine.begin(), reqLine.end(), setReqLine) != 2)
		return (splitReqLine);

	std::size_t	firstSpace = reqLine.find_first_of(setReqLine);
	std::size_t	secondSpace = reqLine.find_first_of(setReqLine, firstSpace + 1);

	splitReqLine.push_back(reqLine.substr(0, firstSpace));
	splitReqLine.push_back(reqLine.substr(firstSpace + 1, secondSpace - (firstSpace + 1)));
	splitReqLine.push_back(reqLine.substr(secondSpace + 1));
	return (splitReqLine);
}

const std::pair<std::string, std::string>	Client::_splitHeaderLine(const std::string &reqLine)
{
	static const std::string	nameValSep = ": ";

	std::pair<std::string, std::string>	splitHeaderLine;

	std::size_t	indexSep = reqLine.find(nameValSep);
	if (indexSep == std::string::npos)
		return (splitHeaderLine);
	splitHeaderLine.first = reqLine.substr(0, indexSep); 
	splitHeaderLine.second = reqLine.substr(indexSep + nameValSep.size()); 
	return (splitHeaderLine);
}

bool	Client::_checkRequestLine(void)	// Add IpPort (to check )
{
	std::vector<std::string>	splitReqLine = _splitRequestLine(_extractedLine);

	if (splitReqLine.empty())
		return (false);
		
	_method = splitReqLine.at(0);
	_requestTarget = Location::simplifyLocationPath(splitReqLine.at(1));
	_HTTPVersion = splitReqLine.at(2);

	if (!(_method == "GET" || _method == "POST" || _method == "DELETE"))
		return (false);
	// Check if method is in IpPort / location
	// check if request target is in IoPort / if [Root]/[RequestTarget] is a dir
	if (_HTTPVersion != _supportedHTTPVersion)
		return (false);
	std::cout << "\e[32m[" << _method << "]\e[33m[" << _requestTarget << "]\e[34m[" << _HTTPVersion << "]\e[0m" << std::endl;
	_response = new Response(this, _config);
	return (true);
}

bool	Client::_checkHeader(void)	// Add IpPort (to check )
{
	if (_extractedLine.empty())
	{
		std::cout  << "\e[1:31m"<< "	-- End Of Header --" << "\e[0m" << std::endl;
		_requestStep = BODY;

		if (_headers.find("Content-Length") == _headers.end() &&
			_headers.find("content-length") == _headers.end())
		{
			_requestStep = FIN;
			std::cout << "\e[1;31m" << "	-- No Body Expected --" << "\e[0m" << std::endl;
		}
		return (true);
	}

	std::pair<std::string, std::string>	_nameVal = _splitHeaderLine(_extractedLine);

	if (_nameVal.first.empty())
		return (false);
	_headers.insert(_nameVal);
	// std::cout  << "\e[35m["<< _nameVal.first << "]\e[34m[" << _nameVal.second  << "]\e[0m" << std::endl;
	return (true);
}

bool	Client::_checkBody(void)	// Add IpPort (to check )
{
	// std::cout  << "\e[1;37m"<< _extractedLine << "\e[0m" << std::endl;
	return (true);
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
		case (FIN):
		{
			std::cout << "FIN" << std::endl;
			return (true);
		}
		default:
			return (false);
	}
}

void Client::checkStep()
{
	if (!_checkCurrentLine(REQUEST_LINE))
	{
		_requestStep = ERROR;
		return ;
	}
	std::cout  << "\e[1:31m"<< "	-- Start Of Header --" << "\e[0m" << std::endl;
	_requestStep = HEADERS;
	while (_requestStep == HEADERS)
	{
		if (!_checkCurrentLine(HEADERS))
		{
			_requestStep = ERROR;
			return ;
		}
	}
	while (_requestStep == BODY)
	{
		if (!_checkCurrentLine(BODY))
		{
			std::cout << CBOLD << _extractedLine << '\n';
			_requestStep = ERROR;
			return ;
		}
	}
	std::cout  << "\e[1:31m"<< "	-- End Of Body --" << "\e[0m" << std::endl;
	_requestStep = FIN;
	if(_requestStep == FIN)
	{
		_response->prepare();
		std::cout << "\033[1;34m" << "\t-- Response ready to be built --" << "\033[0m" << std::endl;
	}
}

void Client::readFromFd()
{
	while(_requestStep != ERROR && _requestStep != FIN)
	{
		checkStep();
	}
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
	return (_requestStep == FIN);
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
