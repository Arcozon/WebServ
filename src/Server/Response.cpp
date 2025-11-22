/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 16:29:20 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/22 17:15:40 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Client.hpp"
#include "Cookies.hpp"

std::map<int, std::string> Response::_reason_phrases;
const std::string	Response::endOfLine = "\r\n";
const std::string	Response::sepNameContent = ": ";

#include "Client.hpp"
#include "FStat.hpp"
#include "ReadDir.hpp"
Response::Response(Client *cl)
: _response_code(200), _send_count(0), _fully_sent(0), _cl(cl)
{
	_body = "";
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


Response::Response(Client *cl, const IpPort &ipPort)
:	_responseCode(200),
	_reasonPhrase("OK"),
	_isCGI(false),
	_send_count(0),
	_fully_sent(false),
	_cl(cl),
	_ipPort(ipPort),
	_location(ipPort.getLocation(cl->getTargetLocation()))
{
	if (_location)
		_URI = _cl->getTargetLocation().substr(_location->getLocation().size());
	_body = "<html><body><h1>";
	_body += "Je suis ";
	_body += _ipPort.getIpPortStr();
	_body += "</h1></body></html>";
}

void Response::setStartLine(int code)
{
	_response_code = code;
	std::ostringstream res;
	res << HTTP_VERSION << " " << code << " " << getReasonPhrase(code) << "\r\n";
	_status_line = res.str();
}

void Response::setHeader(const std::string &key, const std::string &val)
{
	_headers[key] = val;
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

void	Response::prepare(const std::string &body)
{
	_body = body;
	_body += "<html><body><h2>";
	if (_location)
	{
		_body += "Location: "+ _location->getLocation() + endOfLine;
		_body +=  "URI: " + _URI + endOfLine; 

	}
	else
		_body += "Unknown Location [" + _cl->getTargetLocation() + "]" + endOfLine;
	_body += "</h2></body></html>";
	makeRep();
	{
		if (_responseCode == 200)
			_reasonPhrase = "OK";
		else if (_responseCode == 201)
			_reasonPhrase = "Created";
		else if (_responseCode == 204)
			_reasonPhrase = "No Content";
		else if (_responseCode == 301)
			_reasonPhrase = "Moved Permanently";
		else if (_responseCode == 400)
			_reasonPhrase = "Bad Request";
		else if (_responseCode == 403)
			_reasonPhrase = "Forbiden Access";
		else if (_responseCode == 404)
			_reasonPhrase = "Not Found";
		else if (_responseCode == 405)
			_reasonPhrase = "Method Not Allowed";
		else if (_responseCode == 500)
			_reasonPhrase = "Internal Server Error";
		else
			_reasonPhrase = "Unknown Error";
	}
	catResponse();
}

void Response::prepare()
{
	std::ostringstream len, headers;
	if (_headers.find("Content-Length") == _headers.end()  && !_body.empty())
	{
		len << _body.length();
		setHeader("Content-Length", len.str());
	}

	for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it)
		headers << it->first << ": " << it->second << "\r\n";

	for (size_t i = 0; i < _cookies.size(); i++)
		headers << "Set-Cookie: " << _cookies[i].setCookieHeader() << "\r\n";

	headers << "\r\n";
	setStartLine(_response_code);
	_response_buffer += _status_line;
	_response_buffer += headers.str();
	_response_buffer += _body;
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
			else if (_location->hasReturn())
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

void Response::addCookie(const Cookies &cookie)
{
	_cookies.push_back(cookie);
}
