/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IpPort.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:50:09 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/19 11:12:40 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IpPort.hpp"
#include "ParsIpPort.hpp"

IpPort::IpPort(void)
:	_fDefined(0),
	_maxBodySize(0)
{}

IpPort::IpPort(ParsLine &parsLine)
:	_fDefined(0),
	_maxBodySize(0)
{
	ParsIpPort	parsIpPort(parsLine);

	(void)parsIpPort;
}

IpPort::~IpPort(void)
{}
