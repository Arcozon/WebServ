/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:01:27 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/19 13:43:36 by gaeudes          ###   ########.fr       */
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

void WebServ::mkCGIRequest(void)
{
	int fd = open(CGI_FILE_REQ, O_RDWR | O_CREAT | O_TRUNC, 0664);
	char	bodyTest[] = "POST /cgi-bin/CGI_test.py/test?and=a&query=string HTTP/1.1\r\n"
"Host: example.com\r\n"
"User-Agent: TestClient/1.0\r\n"
"Content-Type: application/x-www-form-urlencoded\r\n"
"Content-Length: 9\r\n"
"Connection: close\r\n"
"\r\n"
"name=John\r\n";

	write(fd, bodyTest, strlen(bodyTest));
	close(fd);
}

void WebServ::testCGI(void) const
{
	mkCGIRequest();
	int fd = open(CGI_FILE_REQ, O_RDWR);

	
	Client	cl(fd, _ipPorts[0]);

	cl.readFromFd();
	cl.sendResponse();
}
