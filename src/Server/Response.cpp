/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 16:29:20 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/22 19:00:25 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Client.hpp"
#include "Cookies.hpp"

#include "FStat.hpp"
#include "ReadDir.hpp"

std::map<int, std::string> Response::_reason_phrases;
const std::string	Response::endOfLine = "\r\n";
const std::string	Response::sepNameContent = ": ";

Response::Response(Client *cl, const IpPort &ipPort)
:	_responseCode(200),
	_isCGI(false),
	_insideErr(true),
	_send_count(0),
	_fully_sent(false),
	_cl(cl),
	_ipPort(ipPort),
	_location(NULL)
{
	if(_reason_phrases.empty())
	{
		_reason_phrases[200] = "OK";
		_reason_phrases[201] = "Created";
		_reason_phrases[204] = "No Content";

		_reason_phrases[400] = "Bad Request";
		_reason_phrases[403] = "Forbidden";
		_reason_phrases[404] = "Not Found";
		_reason_phrases[405] = "Method Not Allowed";
		_reason_phrases[411] = "Length Required";
		_reason_phrases[413] = "Payload Too Large";
		_reason_phrases[414] = "URI Too Long";
		_reason_phrases[431] = "Request Header Fields Too Large";

		_reason_phrases[500] = "Internal Server Error";
		_reason_phrases[501] = "Not Implemented";
		_reason_phrases[503] = "Service Unavailable";
		_reason_phrases[505] = "HTTP Version Not Supported";
	}
	setHeader("Server", SERVER_HEADER);
	setHeader("Connection", "close");
	setHeader("Content-Type", "text/html; charset=UTF-8");
}

void Response::setHeader(const std::string &key, const std::string &val)
{
	_header[key] = val;
}

void Response::setBody(const std::string &body)
{
	_body += body;
}

void Response::setLocation(void)
{
	_location = _ipPort.getLocation(_cl->getTargetLocation());
	if (_location)
		std::cout << "Loca:[" << _location->getLocation() << "]\n"; 
	else
		std::cout << "Loca:[NONE]\n"; 
	if (_location)
		_URI = _cl->getTargetLocation().substr(_location->getLocation().size());
}

void	Response::catContentLenght(void)
{
	std::ostringstream	bodySize;
	bodySize << _body.size();
	catHeaderLine("Content-Lenght", bodySize.str());
}

void	Response::catBody(void)
{
	if (!_body.empty())
	{
		catContentLenght();
		catLine("");
		catLine(_body);
	}
}

void	Response::catHeader(void)
{
	typedef	std::map<std::string, std::string>::const_iterator MapStrStrConstIt;

	for (MapStrStrConstIt it = _header.begin(); it != _header.end(); ++it)
		this->catHeaderLine(it->first, it->second);
	for (size_t i = 0; i < _cookies.size(); i++)
		this->catHeaderLine("Set-Cookie: ", _cookies[i].setCookieHeader());

}

void	Response::catCGI(void)
{
	catStatusLine(_responseCode, _reasonPhrase);
	catLine(_body);
}

void	Response::catResponse(void)
{
	if (_isCGI)
	{
		catCGI();
	}
	else
	{
		catStatusLine(_responseCode, _reasonPhrase);
		catHeader();
		catBody();
	}
}

void Response::prepare()
{

	if (_insideErr)
		makeRep();
	else if (isErrorCode(_responseCode))
	{
		_handleError();
	}
	
	catResponse();
}

void Response::send(int fd)
{
	while (_send_count < _response_buffer.length())
	{
		size_t to_send = _response_buffer.length() - _send_count;
		int sent = write(fd, _response_buffer.c_str() + _send_count, to_send);
		if (sent > 0)
		{
			_send_count += sent;
			std::cout << "\e[1;32mSent " << sent << " bytes (Remaining: " << _response_buffer.length() -_send_count 
										<< ")\e[0m" << std::endl;
			_cl->updateTimer();
		}
		else
		{
			std::cout << "\e[1;33mSocket buffer full, waiting for next EPOLLOUT " << "\e[0m" << std::endl;
			return ;
		}
	}
	if(_send_count ==_response_buffer.length())
		_fully_sent = true;
	std::cout << "\e[1;32mResponse sent to client\e[0m" << std::endl;
}

Response::~Response()
{}

bool Response::isResponseFullySent()	const
{
	return (_fully_sent);
}

void	Response::makeRep(void)
{
	Location::sAllowedMethods	methodCode = Location::getMethodCode(_cl->getMethod());
	
	if (methodCode == Location::s_METHODS_MAX)
		_responseCode = 405;
	else if (_location)
	{
		if (!_location->isMethodAllowed(methodCode))
			_responseCode = 405;
		else
		{
			if (_location->_hasCGIHandler(_cl->getTargetLocation()))
				_handleCGI();
			else if (_location->isReturnDefined())
				_handleReturn(_location->getReturn());
			else if (methodCode == Location::s_GET)
				_handleGET();
			else if (methodCode == Location::s_POST)
				_handlePOST();
			else if (methodCode == Location::s_DELETE)
				_handleDELETE();
		}
	}
	else if (_ipPort.hasReturn())
		_handleReturn(_ipPort.getReturn());
	else
		_responseCode = 404;
	if (isErrorCode(_responseCode))
	{
		_handleError();
	}
}

bool	Response::isErrorCode(const unsigned short errCode)
{
	return (errCode >= 400);
}

std::string Response::getReasonPhrase(int code) const
{
	std::map<int, std::string>::iterator it = _reason_phrases.find(code);
	if (it != _reason_phrases.end())
		return it->second;
	else
		return "GAEUDES";
}

void Response::setStartLine(int code)
{
	_responseCode = code;
	_insideErr = false;
}

void Response::addCookie(const Cookies &cookie)
{
	_cookies.push_back(cookie);
}
