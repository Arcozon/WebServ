/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsIpPort.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:33:24 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/19 11:15:30 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParsIpPort.hpp"

const std::string	IpPort::ParsIpPort::_keyHost("host");
const std::string	IpPort::ParsIpPort::_keyPort("port");
const std::string	IpPort::ParsIpPort::_keyServerName("server_name");
const std::string	IpPort::ParsIpPort::_keyClientBodySize("client_max_body_size");
const std::string	IpPort::ParsIpPort::_keyErrorPage("error_page");
const std::string	IpPort::ParsIpPort::_keyLocation("location");
const std::string	IpPort::ParsIpPort::_keyReturn("return");

bool	IpPort::ParsIpPort::_isDefined(alreadyDefined toTest) const
{
	return ((_fDefined & GET_MASK(toTest)) != 0);
}

void	IpPort::ParsIpPort::_addDefined(alreadyDefined toTest)
{
	_fDefined |= GET_MASK(toTest);
}

IpPort::ParsIpPort::~ParsIpPort(void)
{
	printParsServ();
}

IpPort::ParsIpPort::ParsIpPort(ParsLine &parsLine)
:	_parsLine(parsLine),
	_fDefined(0)
{
	while (_parsLine.readLine(_nTabIpPort))
	{
		try
		{
			_addIpPortLine();
		}
		catch(const MyException& e)
		{
			e.throwDown();
			std::cerr << e;
		}			
	}
}

void	IpPort::ParsIpPort::_addIpPortLine(void)
{
	if (_parsLine.getTabDepth() > _nTabIpPort)
		throw (MyException("Too many tabs", MyException::ELVL_ERROR, _parsLine.getLine()));
	
	const std::string	&splitLineFront( _parsLine.getSplitLine().front() );

	if (splitLineFront == _keyHost)
		_addHost();
	else if (splitLineFront == _keyPort)
		_addPort();
	else if (splitLineFront == _keyServerName)
		_addIpPortName();
	else if (splitLineFront == _keyClientBodySize)
		_addClientBodySize();
	else if (splitLineFront == _keyErrorPage)
		_addErrorPage();
	else if (splitLineFront == _keyLocation)
		_addLocation();
	else if (splitLineFront == _keyReturn)
		_addReturn();
	else
		throw (MyException("Unknown key" + _inIpPort(),
			MyException::ELVL_ERROR, splitLineFront));
}

void	IpPort::ParsIpPort::_addHost(void)
{
	const std::vector<std::string>	&splitLine( _parsLine.getSplitLine() );

	if (_isDefined(S_host))
		throw (MyException("Already defined" + _inIpPort(), MyException::ELVL_WARNING, splitLine.front()));
	else if (splitLine.size() != 2)
		throw (MyException("Needs one argument", MyException::ELVL_WARNING, splitLine.front()));
	_host = splitLine.at(1); // TODO: check empty
	_addDefined(S_host);
}

void	IpPort::ParsIpPort::_addPort(void)
{
	const std::vector<std::string>	&splitLine( _parsLine.getSplitLine() );

	if (_isDefined(S_port))
		throw (MyException("Already defined" + _inIpPort(), MyException::ELVL_WARNING, splitLine.front()));
	else if (splitLine.size() != 2)
		throw (MyException("Needs one argument", MyException::ELVL_WARNING, splitLine.front()));
	_port = splitLine.at(1);	// TODO: check value of port
	_addDefined(S_port);
}

void	IpPort::ParsIpPort::_addIpPortName(void)
{
	const std::vector<std::string>	&splitLine( _parsLine.getSplitLine() );

	if (splitLine.size() <= 1)
		throw (MyException("Needs one or more arguments", MyException::ELVL_WARNING, splitLine.front()));
	for (std::vector<std::string>::size_type i = 1; i < splitLine.size(); ++i)
		_IpPortNames.push_back(splitLine.at(i));			// TODO: check duplicates ?
}

void	IpPort::ParsIpPort::_addClientBodySize(void)
{
	const std::vector<std::string>	&splitLine( _parsLine.getSplitLine() );

	if (_isDefined(S_clientBodySize))
		throw (MyException("Already defined" + _inIpPort(), MyException::ELVL_WARNING, splitLine.front()));
	else if (splitLine.size() != 2)
		throw (MyException("Needs one argument", MyException::ELVL_WARNING, splitLine.front()));
	_clientMaxBodySize = splitLine.at(1);	// TODO: Check val, convert to int
	_addDefined(S_clientBodySize);
}

void	IpPort::ParsIpPort::_addErrorPage(void)
{
	const std::vector<std::string>	&splitLine( _parsLine.getSplitLine() );

	if (splitLine.size() != 3)
		throw (MyException("Needs two arguments", MyException::ELVL_WARNING, splitLine.front()));
	else if (!_isHTTPErrorCode(splitLine.at(1)))
		throw (MyException("Not a valid HTTP error code", MyException::ELVL_WARNING, splitLine.at(1)));
	else if (_errorPages.find(splitLine.at(1)) != _errorPages.end())
		throw (MyException("Error page is already defined", MyException::ELVL_WARNING, splitLine.at(1)));
	_errorPages.insert(std::pair<std::string, std::string>(splitLine.at(1), splitLine.at(2)));
}

void	IpPort::ParsIpPort::_addLocation(void)
{
	const std::vector<std::string>	&splitLine( _parsLine.getSplitLine() );

	if (splitLine.size() != 2)
		throw (MyException("Needs one argument", MyException::ELVL_WARNING, splitLine.front()));
	_locations.push_back(Location(_parsLine));
}

void	IpPort::ParsIpPort::_addReturn(void)
{
	const std::vector<std::string>	&splitLine( _parsLine.getSplitLine() );

	if (_isDefined(S_return))
		throw (MyException("Already defined" + _inIpPort(), MyException::ELVL_WARNING, splitLine.front()));
	else if (splitLine.size() != 2 && splitLine.size() != 3)
		throw (MyException("Needs one or two arguments", MyException::ELVL_WARNING, splitLine.front()));
	else if (splitLine.size() == 2)
		_return = Return(splitLine.at(1));
	else if (splitLine.size() == 3) 
		_return = Return(splitLine.at(1), splitLine.at(2));
	_addDefined(S_return);
}

bool	IpPort::ParsIpPort::_isServValid(void) const
{
	return (true);
}

void	IpPort::ParsIpPort::printParsServ(void) const
{
	std::cout << "Host: " << _host << '\n';
	std::cout << "Port: " << _port << '\n';
	std::cout << "IpPortNames: ";
	{
		for (std::vector<std::string>::const_iterator it = _IpPortNames.begin(); it != _IpPortNames.end(); ++it)
			std::cout << *it << "  ";
		if (_IpPortNames.size() == 0)
			std::cout << "	" << "None";
		std::cout << '\n';
	}
	std::cout << "ClientMaxSizeBody: " << (_isDefined(S_clientBodySize) ? _clientMaxBodySize : "Not defined") << '\n';
	std::cout << "ErrorPages: " << '\n';
	{
		for (std::map<std::string, std::string>::const_iterator it = _errorPages.begin();
				it != _errorPages.end(); ++it)
			std::cout << "	" << it->first << ": " << it->second << '\n';
		if (_errorPages.size() == 0)
			std::cout << "	" << "Empty" << '\n';
	}
	std::cout << "Locations: " << '\n';
	{
		for (std::vector<Location>::const_iterator it = _locations.begin(); it != _locations.end(); ++it)
			std::cout << "	" << "not added" << '\n';
		if (_locations.size() == 0)
			std::cout << "	" << "Empty" << '\n';
	}
	std::cout << "Return: ";
	_return._printInfo();
	std::cout << std::endl;
}
