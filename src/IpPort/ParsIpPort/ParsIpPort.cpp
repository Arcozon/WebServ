/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsIpPort.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:33:24 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/19 12:30:32 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParsIpPort.hpp"

const std::string	IpPort::ParsIpPort::_keyHost("host");
const std::string	IpPort::ParsIpPort::_keyLocalHost("localhost");
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

bool	IpPort::ParsIpPort::_isValidNumInRange0to255(const std::string& str)
{
	const std::size_t	strSize = str.size();
	
	if (strSize == 0 || strSize > 3)
		return (false);
	for (std::string::const_iterator cit = str.begin(); cit != str.end(); ++cit)
		if (!std::isdigit(*cit))
			return  (false);
	
	const int	toInt = std::atoi(str.c_str());

	return (toInt <= 255);
}

bool	IpPort::ParsIpPort::_isValidIP(const std::string &hostStr) const
{
	static const int	_NDotInIP = 3;
	if (hostStr == _keyLocalHost)
		return (true);
	if (std::count(hostStr.begin(), hostStr.end(), '.') != _NDotInIP)
		return (false);
	
	std::vector<std::string>	hostSplit = ParsLine::splitLine(hostStr, '.', false);

	if (hostSplit.size() != (_NDotInIP + 1))
		return (false);

	for (std::vector<std::string>::iterator it = hostSplit.begin(); it != hostSplit.end(); ++it)
		if (!_isValidNumInRange0to255(*it))
			return (false);
	return (true);
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
	
	std::string TmpHostStr = splitLine.at(1);
	
	if (!_isValidIP(TmpHostStr))
		throw (MyException("Invalid host format", MyException::ELVL_ERROR, TmpHostStr));
	_hostStr = TmpHostStr;
	_addDefined(S_host);
}

void	IpPort::ParsIpPort::_addPort(void)
{
	const std::vector<std::string>	&splitLine( _parsLine.getSplitLine() );

	if (_isDefined(S_port))
		throw (MyException("Already defined" + _inIpPort(), MyException::ELVL_WARNING, splitLine.front()));
	else if (splitLine.size() != 2)
		throw (MyException("Needs one argument", MyException::ELVL_WARNING, splitLine.front()));
	_portStr = splitLine.at(1);	// TODO: check value of port
	_addDefined(S_port);
}

void	IpPort::ParsIpPort::_addIpPortName(void)
{
	const std::vector<std::string>	&splitLine( _parsLine.getSplitLine() );

	if (splitLine.size() <= 1)
		throw (MyException("Needs one or more arguments", MyException::ELVL_WARNING, splitLine.front()));
	for (std::vector<std::string>::size_type i = 1; i < splitLine.size(); ++i)
		_ServerNames.push_back(splitLine.at(i));			// TODO: check duplicates ?
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
	std::cout << "Host: " << _hostStr << '\n';
	std::cout << "Port: " << _portStr << '\n';
	std::cout << "ServerNames: ";
	{
		for (std::vector<std::string>::const_iterator it = _ServerNames.begin(); it != _ServerNames.end(); ++it)
			std::cout << *it << "  ";
		if (_ServerNames.size() == 0)
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
