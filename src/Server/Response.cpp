/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 16:29:20 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/03 15:52:51 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Client.hpp"

const std::string	Response::endOfLine = "\r\n";
const std::string	Response::sepNameContent = ": ";

Response::Response(Client *cl, const IpPort &ipPort)
:	_responseCode(201),
	_reasonPhrase("OK"),
	_send_count(0),
	_fully_sent(false),
	_cl(cl),
	_ipPort(ipPort),
	_location(ipPort.getLocation(cl->getTargetLocation()))
{
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
}

void	Response::catHeader(void)
{
	typedef	std::map<std::string, std::string>::const_iterator CIttMapHeader;

	for (CIttMapHeader it = _header.begin(); it != _header.end(); ++it)
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
		_body += "Est une location valide: "+ _location->getLocation() + endOfLine;
		_body +=  "Reste: " + _cl->getTargetLocation().substr(_location->getLocation().size()) + endOfLine; 

	}
	else
		_body += "Je connais pas [" + _cl->getTargetLocation() + "]" + endOfLine;
	_body += "</h2></body></html>";
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
