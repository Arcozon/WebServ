/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IpPort.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:50:09 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/25 15:28:56 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IpPort.hpp"
#include "ParsIpPort.hpp"

IpPort::IpPort(void)
:	_fDefined(0),
	_host(0),
	_port(0),
	_clientMaxBodySize(0)
{}

IpPort::IpPort(ParsLine &parsLine)
:	_fDefined(0),
	_clientMaxBodySize(0)
{
	ParsIpPort	parsIpPort(parsLine);

	if (!parsIpPort._isIpPortValid())
		return ;

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

