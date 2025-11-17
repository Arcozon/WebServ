/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:01:27 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/14 15:36:32 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include "ParsWebServ.hpp"

WebServ::WebServ(const int ac, char *av[], char *env[])
:	_env(env), _init(false)
{
	ParsWebServ	parsIpPorts(ac, av);
	
	_ipPorts = parsIpPorts.getIpPorts();
	if (_ipPorts.empty())
		throw (MyException("No valid server", MyException::ELVL_FATAL, "Config"));

	for (unsigned int i = 0; i < _ipPorts.size(); ++i)
		std::cout << i << ": " << _ipPorts[i].getIpPortStr() << '\n';
}

WebServ::~WebServ(void)
{}

std::vector<IpPort> &WebServ::getServers()
{
	return (_ipPorts);
}
