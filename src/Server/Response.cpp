/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 16:29:20 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/07 19:14:57 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

#include "Client.hpp"
#include "FStat.hpp"
#include "ReadDir.hpp"

const std::string	Response::endOfLine = "\r\n";
const std::string	Response::sepNameContent = ": ";

Response::Response(Client *cl, const IpPort &ipPort)
:	_responseCode(200),
	_reasonPhrase("OK"),
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

void	Response::setHeaders(const std::map<std::string, std::string> &map)
{
	// _header.insert("Content-Type", "text/html; charset=UTF-8");
	_header.insert(std::make_pair("Content-Type", "text/html; charset=UTF-8"));
	_header.insert(std::make_pair("Connection", "close"));
	(void)map;
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
		catLine(_body);	// remplacer les \n par des \r\n ?
	}
	{
		std::ostringstream oss;
		oss << _responseCode;
		catLine(oss.str() + " " + _reasonPhrase);
	}
}

void	Response::catHeader(void)
{
	typedef	std::map<std::string, std::string>::const_iterator MapStrStrConstIt;

	for (MapStrStrConstIt it = _header.begin(); it != _header.end(); ++it)
		this->catHeaderLine(it->first, it->second);
}

void	Response::catResponse(void)
{
	catStatusLine(_responseCode, _reasonPhrase);
	catHeader();
	catBody();
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
			_reasonPhrase = "WTF";
	}
	catResponse();
}

void Response::prepare()
{
	prepare(_body);
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
			std::cout << "\e[1;32mSent " << sent << " bytes (Remaining: ";
			std::cout << _response_buffer.length() - _send_count << ")\e[0m" << std::endl;
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
			if (methodCode == Location::s_GET)
				_handleGET();
			else if (methodCode == Location::s_POST)
				_handlePOST();
			else if (methodCode == Location::s_DELETE)
				_handleDELETE();

		}
	}
	else
		_responseCode = 404;
}
