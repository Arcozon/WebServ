/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:33:24 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/15 18:16:01 by gaeudes          ###   ########.fr       */
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

bool	Server::ParsServer::_isDefined(alreadyDefined toTest) const
{
	return ((_fDefined & GET_MASK(toTest)) != 0);
}

void	Server::ParsServer::_addDefined(alreadyDefined toTest)
{
	_fDefined |= GET_MASK(toTest);
}

Server::ParsServer::~ParsServer(void)
{}

Server::ParsServer::ParsServer(std::ifstream &configFile, std::string &line)
:	_configFile(configFile),
	_line(line),
	_fDefined(0)
{
	while (std::getline(_configFile, _line))
	{
		if (_isLineEmpty(_line))
			continue ;
		else if (_getTabDepth(_line) < _nTabServer)
			break ;
		try
		{
			_addServerLine();
		}
		catch(const MyException& e)
		{
			std::cerr << e;
		}			
	}
}

void	Server::ParsServer::_addServerLine(void)
{
	if (_getTabDepth(_line) > _nTabServer)
		throw (MyException("Too many tabs", MyException::ELVL_ERROR, _line));
	
	_splitLine = _split(_line);
	if (_splitLine.front() == _keyHost)
		_addHost();
	else if (_splitLine.front() == _keyPort)
		_addPort();
	else if (_splitLine.front() == _keyServerName)
		_addServerName();
	else if (_splitLine.front() == _keyClientBodySize)
		_addClientBodySize();
	else if (_splitLine.front() == _keyErrorPage)
		_addErrorPage();
	else if (_splitLine.front() == _keyLocation)
		_addLocation();
	else if (_splitLine.front() == _keyReturn)
		_addReturn();
	else
		throw (MyException("Unknow key", MyException::ELVL_ERROR, _splitLine.front()));
}

void	Server::ParsServer::_addHost(void)
{
	if (_isDefined(S_host))
		throw (MyException("Already defined", MyException::ELVL_ERROR, _splitLine.front()));
	else if (_splitLine.size() != 2)
		throw (MyException("Needs one argument", MyException::ELVL_ERROR, _splitLine.front()));
	_host = _splitLine[1]; // TODO: check empty
	_addDefined(S_host);
}

void	Server::ParsServer::_addPort(void)
{
	if (_isDefined(S_port))
		throw (MyException("Already defined", MyException::ELVL_ERROR, _splitLine.front()));
	else if (_splitLine.size() != 2)
		throw (MyException("Needs one argument", MyException::ELVL_ERROR, _splitLine.front()));
	_port = _splitLine[1];	// TODO: check value of port
	_addDefined(S_port);
}

void	Server::ParsServer::_addServerName(void)
{
	if (_splitLine.size() <= 1)
		throw (MyException("Needs one or more arguments", MyException::ELVL_ERROR, _splitLine.front()));
	for (std::size_t i = 1; i < _splitLine.size(); ++i)
		_serverNames.push_back(_splitLine[i]);			// TODO: check duplicates ?
}

void	Server::ParsServer::_addClientBodySize(void)
{
	if (_isDefined(S_clientDodySize))
		throw (MyException("Already defined", MyException::ELVL_ERROR, _splitLine.front()));
	else if (_splitLine.size() != 2)
		throw (MyException("Needs one argument", MyException::ELVL_ERROR, _splitLine.front()));
	_clientMaxBodySize = _splitLine[1];	// TODO: Check val, convert to int
	_addDefined(S_clientDodySize);
}

void	Server::ParsServer::_addErrorPage(void)
{
	if (_splitLine.size() != 3)
		throw (MyException("Needs two arguments", MyException::ELVL_ERROR, _splitLine.front()));
	else if (!_isHTTPErrorCode(_splitLine[1]))
		throw (MyException("Not a valid HTTP error code", MyException::ELVL_ERROR, _splitLine[1]));
	else if (_errorPages.find(_splitLine[1]) != _errorPages.end())
		throw (MyException("Error page is already defined", MyException::ELVL_ERROR, _splitLine[1]));
	_errorPages.insert(std::pair<std::string, std::string>(_splitLine[1], _splitLine[2]));
}

void	Server::ParsServer::_addLocation(void)
{
	if (_splitLine.size() != 2)
		throw (MyException("Needs one argument", MyException::ELVL_ERROR, _splitLine.front()));
	// 
	// add location
}

void	Server::ParsServer::_addReturn(void)
{
	if (_isDefined(S_return))
		throw (MyException("Already defined", MyException::ELVL_ERROR, _splitLine.front()));
	else if (_splitLine.size() != 2 && _splitLine.size() != 3)
		throw (MyException("Needs one or two arguments", MyException::ELVL_ERROR, _splitLine.front()));
	_addDefined(S_return);
}

bool	Server::ParsServer::_isServValid(void) const
{
	return (true);
}
