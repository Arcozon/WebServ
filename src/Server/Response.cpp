/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 16:29:20 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/04 14:01:56 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

#include "Client.hpp"
#include "FStat.hpp"
#include "ReadDir.hpp"

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
	mkRepFromLoc();
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

void	Response::fileToBody(char const fName[])
{
	int fd = open(fName, 0);
	if (fd < 0)
		return ;	//Err
	_body.clear();
	const int	toRead = 1024;		
	char		buffer[toRead];
	int			br(toRead);
	while (br)
	{
		br = read(fd, buffer, toRead);
		if (br < 0)
			return ;
		_body.append(buffer, br);
	}
	close(fd);
}

bool	Response::lookForIndex(const char dName[])	// Returns true if one index was found
{
	typedef	std::vector<std::string>::const_iterator	VecStrConstIt;
	const std::vector<std::string> indexs = _location->getIndexs();

	FStat	fileStat;

	for (VecStrConstIt it = indexs.begin(); it != indexs.end(); ++it)
	{
		fileStat.open(dName, *it);
		if (fileStat.isFile())
		{
			fileToBody(fileStat.getPathCStr());
			return (true);
		}
	}
	return (false);
}

void	Response::mkRepFromLoc(void)
{
	if (!_location)
		return ;
	//	Handle return
	FStat	fileStat(_location->getRoot(), _URI);
	// std::cout << fileStat.getPathCStr() << std::endl;
	if (fileStat.isFile())
		fileToBody(fileStat.getPathCStr());
	else if (fileStat.isDir())
	{
		if (!lookForIndex(fileStat.getPathCStr()))
		{
			if (_location->autoIndexOn())
			{
				generateAutoIndex(fileStat.getPathCStr());
			}
			//err
		}
	}
	else
		;	// Err
}
