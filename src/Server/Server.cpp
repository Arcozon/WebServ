/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:50:09 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/14 18:01:31 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::initSockets()
{
	for (size_t i = 0; i < _ports.size(); i++)
	{
		int fd = socket(AF_INET, SOCK_STREAM, 0); // necessaire pour recevoir et envoyer des informations (par la que tout arrive)
		if (fd == -1)
			throw std::runtime_error("socket() syscall failed");

		int flags = fcntl(fd, F_GETFL, 0); // rends les sockets non bloquants, doit être appelé pour chaque fd (potentiellement à enlever selon le sujet)
		fcntl(fd, F_SETFL, flags | O_NONBLOCK);

		int opt = 1;
		if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) // https://stackoverflow.com/a/69923308
			throw std::runtime_error("setsockopt() syscall failed");
		
		sockaddr_in addr;
		std::memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET; // IPv4
		addr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
		addr.sin_port = htons(_ports[i]);

		if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
		{
			std::ostringstream oss;
			oss << "bind() syscall failed to listen on port: " << _ports[i];
			throw std::runtime_error(oss.str());
		}

		if (listen(fd, SOMAXCONN) == -1)
			throw std::runtime_error("listen() syscall failed to accept incoming connections on port: ");
		
		if (listen(fd, SOMAXCONN) == -1)
			throw std::runtime_error("listen() syscall failed to accept incoming connections on port: ");

		this->_epoll_fds.push_back(fd); // fd qu'on va monitor avec epoll
	}
}

void Server::initEpoll()
{
	_epoll_instance = epoll_create1(0);
	if (_epoll_instance == -1)
		throw std::runtime_error("Failed to create epoll instance (epoll_create1)");
	
	for(size_t i = 0; i < _epoll_fds.size(); i++)
	{
		epoll_event ev;
		std::memset(&ev, 0, sizeof(ev));
		ev.events = EPOLLIN | EPOLLET; // Flags pour monitor les events d'arrivées (EPOLLIN) et edge-triggered (non bloquants (EPOLLET))
		ev.data.fd = _epoll_fds[i];
		if (epoll_ctl(_epoll_instance, EPOLL_CTL_ADD, _epoll_fds[i], &ev) == -1)
			throw std::runtime_error("Failed to create epoll control interface (epoll_ctl");
	}
}

void Server::print_fds()
{
	for(size_t i = 0; i < _epoll_fds.size(); i++)
		std::cout << "_epoll_fds[" << i << "] = " << _epoll_fds[i] << std::endl;
}

Server::Server(void)
{
	_ports.push_back(8080);
	_ports.push_back(8181);
	_ports.push_back(8282);
	initSockets();
	initEpoll();
}

Server::~Server(void)
{
	for(size_t i = 0; i < _epoll_fds.size(); i++)
		close(_epoll_fds[i]);
	close(_epoll_instance);
}
