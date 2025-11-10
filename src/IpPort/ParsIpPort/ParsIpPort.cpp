/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsIpPort.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:33:24 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/10 17:54:02 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParsIpPort.hpp"

const std::string	IpPort::ParsIpPort::_keyHost("host");
const std::string	IpPort::ParsIpPort::_keyLocalHost("localhost");
const std::string	IpPort::ParsIpPort::_keyPort("port");
const std::string	IpPort::ParsIpPort::_keyClientBodySize("client_max_body_size");
const std::string	IpPort::ParsIpPort::_keyErrorPage("error_page");
const std::string	IpPort::ParsIpPort::_keyLocation("location");
const std::string	IpPort::ParsIpPort::_keyReturn("return");

bool	IpPort::ParsIpPort::_isDefined(sDefined toTest) const
{
	return ((_fDefined & GET_MASK(toTest)) != 0);
}

void	IpPort::ParsIpPort::_addDefined(sDefined toTest)
{
	_fDefined |= GET_MASK(toTest);
}

IpPort::ParsIpPort::~ParsIpPort(void)
{
	// printParsServ();
}

bool	IpPort::ParsIpPort::_isValidNumInRange0to255(const std::string& str)
{
	const std::size_t	strSize = str.size();
	
	if (strSize == 0 || strSize > 3)
		return (false);
	for (std::string::const_iterator cit = str.begin(); cit != str.end(); ++cit)
		if (!std::isdigit(*cit))
			return (false);
	
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
	
	const std::vector<std::string>	hostSplit = ParsLine::splitLine(hostStr, '.', false);

	if (hostSplit.size() != (_NDotInIP + 1))
		return (false);

	for (std::vector<std::string>::const_iterator it = hostSplit.begin(); it != hostSplit.end(); ++it)
		if (!_isValidNumInRange0to255(*it))
			return (false);
	return (true);
}

unsigned long	IpPort::ParsIpPort::_IPStrToUL(const std::string &hostStr)	const
{
	const std::vector<std::string>	hostSplit = ParsLine::splitLine(hostStr, '.', false);
	unsigned long	res(0);

	for (std::vector<std::string>::const_iterator it = hostSplit.begin(); it != hostSplit.end(); ++it)
	{
		res <<= 8;
		res |= std::atoi(it->c_str());
	}
	return (res);
}

bool	IpPort::ParsIpPort::_isValidPort(const std::string &portStr) const
{
	static const int	_portMax = 65535;

	for (std::string::const_iterator cit = portStr.begin(); cit != portStr.end(); ++cit)
		if (!std::isdigit(*cit))
			return (false);
	return (std::atoi(portStr.c_str()) < _portMax);
}

IpPort::ParsIpPort::ParsIpPort(ParsLine &parsLine)
:	_parsLine(parsLine),
	_fDefined(0),
	_clientMaxBodySize(0),
	_valid(true)
{
	while (_parsLine.readLine(_nTabIpPort))
	{
		if (_valid)
		{
			try
			{
				_addIpPortLine();
			}
			catch(const MyException& e)
			{
				e.throwDown();
				std::cerr << e;
				if (e.getErrLvl() == MyException::ELVL_ERROR)
					_valid = false;
			}			
		}
	}
}

void	IpPort::ParsIpPort::_addIpPortLine(void)
{
	if (_parsLine.getTabDepth() > _nTabIpPort)
		throw (MyException("Too many tabs", MyException::ELVL_WARNING, _parsLine.getLine()));
	
	const std::string	&splitLineFront( _parsLine.getSplitLine().front() );

	if (splitLineFront == _keyHost)
		_addHost();
	else if (splitLineFront == _keyPort)
		_addPort();
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
			MyException::ELVL_WARNING, splitLineFront));
}

void	IpPort::ParsIpPort::_addHost(void)
{
	const std::vector<std::string>	&splitLine( _parsLine.getSplitLine() );

	if (_isDefined(s_host))
		throw (MyException("Already defined" + _inIpPort(), MyException::ELVL_WARNING, splitLine.front()));
	else if (splitLine.size() != 2)
		throw (MyException("Needs one argument", MyException::ELVL_WARNING, splitLine.front()));
	
	std::string TmpHostStr = splitLine.at(1);
	
	if (!_isValidIP(TmpHostStr))
		throw (MyException("Invalid host format", MyException::ELVL_ERROR, TmpHostStr));
	_hostStr = TmpHostStr;
	_host = _IPStrToUL(_hostStr);
	_addDefined(s_host);
}

void	IpPort::ParsIpPort::_addPort(void)
{
	const std::vector<std::string>	&splitLine( _parsLine.getSplitLine() );

	if (_isDefined(s_port))
		throw (MyException("Already defined" + _inIpPort(), MyException::ELVL_WARNING, splitLine.front()));
	else if (splitLine.size() != 2)
		throw (MyException("Needs one argument", MyException::ELVL_WARNING, splitLine.front()));
	
	std::string TmpPortStr = splitLine.at(1);
	
	if (!_isValidPort(TmpPortStr))
		throw (MyException("Invalid port format", MyException::ELVL_ERROR, TmpPortStr));
	_portStr = TmpPortStr;
	_port = std::atoi(TmpPortStr.c_str());
	_addDefined(s_port);
}

void	IpPort::ParsIpPort::_addClientBodySize(void)
{
	const std::vector<std::string>	&splitLine( _parsLine.getSplitLine() );

	if (_isDefined(s_clientBodySize))
		throw (MyException("Already defined" + _inIpPort(), MyException::ELVL_WARNING, splitLine.front()));
	else if (splitLine.size() != 2)
		throw (MyException("Needs one argument", MyException::ELVL_WARNING, splitLine.front()));

	std::string	strCMBS = splitLine.at(1);

	for (std::string::iterator it = strCMBS.begin(); it != strCMBS.end(); ++it)
		if (!std::isdigit(*it))
			throw (MyException("Invalid client_max_body_size format", MyException::ELVL_ERROR, strCMBS));
	_clientMaxBodySize = std::atol(strCMBS.c_str());	// TODO: check if 0?
	_addDefined(s_clientBodySize);
}

void	IpPort::ParsIpPort::_addErrorPage(void)
{
	const std::vector<std::string>	&splitLine( _parsLine.getSplitLine() );

	if (splitLine.size() != 3)
		throw (MyException("Needs two arguments", MyException::ELVL_WARNING, splitLine.front()));
	else if (!Return::_isHTTPErrorCode(splitLine.at(1)))
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
	
	Location	location(_parsLine);

	if (!location.isValid())
		throw (MyException("Invalid location", MyException::ELVL_ERROR, std::string()));

	_locations.push_back(location);
}

void	IpPort::ParsIpPort::_addReturn(void)
{
	const std::vector<std::string>	&splitLine( _parsLine.getSplitLine() );

	if (_isDefined(s_return))
		throw (MyException("Already defined" + _inIpPort(), MyException::ELVL_WARNING, splitLine.front()));
	else if (splitLine.size() < 2)
		throw (MyException("Needs one or more arguments", MyException::ELVL_WARNING, splitLine.front()));
	else if (splitLine.size() == 2)
		_return = Return(splitLine.at(1));
	else
	{
		const std::string	&line = _parsLine.getLine();
		const std::size_t	endWord1 = line.find(splitLine.at(1)) + splitLine.at(1).size();
		const std::size_t	startRest = line.find_first_not_of(' ', endWord1);

		_return = Return(splitLine.at(1), line.substr(startRest));
	}
	_addDefined(s_return);
}

bool	IpPort::ParsIpPort::_isIpPortValid(void) const
{
	if (!_isDefined(s_host) || !_isDefined(s_port))
		return (false);
	return (true);
}

void	IpPort::ParsIpPort::printParsServ(void) const
{
	std::cout << "Host: " << _hostStr << '\n';
	std::cout << "Port: " << _portStr << '\n';
	std::cout << "ClientMaxSizeBody: ";
	if (_isDefined(s_clientBodySize))
		std::cout << _clientMaxBodySize <<'\n';
	else
		std::cout << "Not defined" << '\n';
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

unsigned long	IpPort::ParsIpPort::getIpPortFlag(void) const
{
	return (_fDefined);
}

const std::string	&IpPort::ParsIpPort::getStrHost(void) const
{
	return (_hostStr);
}

unsigned long	IpPort::ParsIpPort::getHost(void) const
{
	return (_host);
}

const std::string	&IpPort::ParsIpPort::getStrPort(void) const
{
	return (_portStr);
}

unsigned long	IpPort::ParsIpPort::getPort(void) const
{
	return (_port);
}

unsigned long	IpPort::ParsIpPort::getClientMaxBodySize(void) const
{
	return (_clientMaxBodySize);
}

const std::map<std::string, std::string>	&IpPort::ParsIpPort::getErrorPages(void) const
{
	return (_errorPages);
}

const std::vector<Location>	&IpPort::ParsIpPort::getLocations(void) const
{
	return (_locations);
}

const Return	&IpPort::ParsIpPort::getReturn(void) const
{
	return (_return);
}
