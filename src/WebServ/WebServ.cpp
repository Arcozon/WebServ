/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:01:27 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/17 12:12:41 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include "ParsWebServ.hpp"

WebServ::WebServ(const int ac, char *av[], char *env[])
:	_env(env), _init(false)
{
	ParsWebServ	parsIpPorts(ac, av);
	
	_ipPorts = parsIpPorts.getIpPorts();
	_init = true;
	for (unsigned int i = 0; i < _ipPorts.size(); ++i)
		std::cout << i << ": " << _ipPorts[i].getIpPortStr() << '\n';
}

WebServ::~WebServ(void)
{}

std::vector<IpPort> &WebServ::getServers()
{
	return (_ipPorts);
}

# include <unistd.h>
# include <fcntl.h>

# include "Client.hpp"

#define CGI_FILE_REQ "CGIRequest.cgi"

// void WebServ::mkCGIRequest(void)
// {
// 	int fd = open(CGI_FILE_REQ, O_RDWR | O_CREAT | O_TRUNC, 0664);
// 	char	bodyTest = ""
// 	// Client 
// }

void WebServ::testCGI(void) const
{
}
