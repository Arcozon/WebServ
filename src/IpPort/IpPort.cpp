/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IpPort.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:50:09 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/10 18:17:20 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IpPort.hpp"
#include "ParsIpPort.hpp"

IpPort::IpPort(void)
:	_fDefined(0),
	_host(0),
	_port(0),
	_clientMaxBodySize(0),
	_valid(false)
{}

IpPort::IpPort(ParsLine &parsLine)
:	_fDefined(0),
	_host(0),
	_port(0),
	_clientMaxBodySize(0),
	_valid(false)
{
	ParsIpPort	parsIpPort(parsLine);

	if (!parsIpPort._isIpPortValid())
		return ;
	_valid = true;
	_fDefined = parsIpPort.getIpPortFlag();
	_StrHost = parsIpPort.getStrHost();
	_host = parsIpPort.getHost();
	_StrPort = parsIpPort.getStrPort();
	_port = parsIpPort.getPort();
	_strHostPort = _StrHost + ':' + _StrPort;
	_clientMaxBodySize = parsIpPort.getClientMaxBodySize();
	_locations = parsIpPort.getLocations();
	_errPages = parsIpPort.getErrorPages();

	_return = parsIpPort.getReturn();
	_return._fixUrlWithDomainName(_strHostPort);

	typedef std::vector<Location>::iterator	VecLocIt;

	for (VecLocIt it = _locations.begin(); it != _locations.end(); ++it)	
		it->fixReturn(_strHostPort);
}

IpPort::~IpPort(void)
{}

bool	IpPort::isValid(void) const
{
	return (_valid);
}

const std::string	&IpPort::getIpPortStr(void) const
{
	return (_strHostPort);
}

const std::string &IpPort::getHostStr(void) const
{
	return (_StrHost);
}

const std::string &IpPort::getPortStr(void) const
{
	return (_StrPort);
}

const unsigned long &IpPort::getHost(void) const
{
	return (_host);
}

const unsigned short &IpPort::getPort(void) const
{
	return (_port);
}
static inline bool	_isLocationMatchingWPath(const std::string& loc, const std::string &path)
{
	if (loc.find(path) == 0)
	{
		if (path.size() == loc.size())
			return (true);
		else if (loc[path.size()] == '/')
			return (true);
	}
	return (false);
}

const Location	*IpPort::getLocation(const std::string &cPath)	const
{
	typedef std::vector<Location>::const_iterator	VecLocConstIt;

	std::string	path = Location::simplifyLocationPath(cPath);
	
	while (!path.empty())
	{
		for (VecLocConstIt it = _locations.begin(); it != _locations.end(); ++it)
		{
			if (_isLocationMatchingWPath(it->getLocation(), path))
				return (it.base());
		}
		if (path[path.size() - 1] == '/')
		{
			path.erase(path.size() - 1);
		}
		else
		{
			std::size_t	lastSlash = path.find_last_of('/');
			if (lastSlash == std::string::npos)
				path.clear();
			else
				path.erase(lastSlash + 1);
		}
	}
	return (NULL);
}

bool IpPort::errorPageDefined(const std::string &errCode) const
{
	typedef std::map<std::string, std::string>::const_iterator	mapStrStrCIt;

	mapStrStrCIt	itErr = _errPages.find(errCode);
	return (itErr != _errPages.end());
}

bool IpPort::errorPageDefined(const std::size_t &errCode) const
{
	std::stringstream	sStrCode;

	sStrCode << errCode;
	return (errorPageDefined(sStrCode.str()));
}

const std::string	&IpPort::getErrorPage(const std::string &errCode) const
{
	typedef std::map<std::string, std::string>::const_iterator	mapStrStrCIt;

	mapStrStrCIt	itErr = _errPages.find(errCode);
	return (itErr->second);
}

const std::string	&IpPort::getErrorPage(const std::size_t &errCode) const
{
	std::stringstream	sStrCode;

	sStrCode << errCode;
	return (getErrorPage(sStrCode.str()));
}
