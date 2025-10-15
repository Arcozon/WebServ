/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:33:24 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/15 17:29:42 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParsServer.hpp"

const std::string	Server::ParsServer::_keyHost("host");
const std::string	Server::ParsServer::_keyPort("port");
const std::string	Server::ParsServer::_keyServerName("server_name");
const std::string	Server::ParsServer::_keyClientBodySize("client_max_body_size");
const std::string	Server::ParsServer::_keyErrorPage("error_page");
const std::string	Server::ParsServer::_keyLocation("location");
const std::string	Server::ParsServer::_keyReturn("return");

Server::ParsServer::~ParsServer(void)
{}

Server::ParsServer::ParsServer(std::ifstream &configFile, std::string &line)
:	_configFile(configFile), _line(line)
{
	while (std::getline(_configFile, _line))
	{
		if (_isLineEmpty(_line))
			continue ;
		else if (_getTabDepth(_line) < _nTabServer)
			break ;
		else if (_getTabDepth(_line) > _nTabServer)
			throw (MyException("Too many tabs", MyException::ELVL_FATAL, line));
		else
			_addServerLine();
	}
}

void	Server::ParsServer::_addServerLine(void)
{
	_splitLine = _split(_line);

	if (_splitLine[0] == _keyHost)
		_addHost();
	else if (_splitLine[0] == _keyPort)
		_addPort();
	else if (_splitLine[0] == _keyServerName)
		_addServerName();
	else if (_splitLine[0] == _keyClientBodySize)
		_addClientBodySize();
	else if (_splitLine[0] == _keyErrorPage)
		_addErrorPage();
	else if (_splitLine[0] == _keyLocation)
		_addLocation();
	else if (_splitLine[0] == _keyReturn)
		_addReturn();
}

void	Server::ParsServer::_addHost(void)
{

}

void	Server::ParsServer::_addPort(void)
{

}

void	Server::ParsServer::_addServerName(void)
{

}

void	Server::ParsServer::_addClientBodySize(void)
{

}

void	Server::ParsServer::_addErrorPage(void)
{

}

void	Server::ParsServer::_addLocation(void)
{

}

void	Server::ParsServer::_addReturn(void)
{

}

bool	Server::ParsServer::_isServValid(void) const
{
	return (true);
}
