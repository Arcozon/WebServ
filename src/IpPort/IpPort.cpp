/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IpPort.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:50:09 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/30 14:51:49 by gaeudes          ###   ########.fr       */
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
	_IpPortNames = parsIpPort.getServerNames();
	_clientMaxBodySize = parsIpPort.getClientMaxBodySize();
	_locations = parsIpPort.getLocations();
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
const std::string &IpPort::getHost(void) const
{
	return _host;
}

const std::string &IpPort::getPort(void) const
{
	return _port;
}
