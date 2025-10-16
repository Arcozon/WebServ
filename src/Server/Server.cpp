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
		{
			std::ostringstream oss;
			oss << "listen() syscall failed to accept incoming connections on port: " << _ports[i];
			throw std::runtime_error(oss.str());
		}

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

bool Server::is_registered_socket(int fd)
{
	for(size_t i = 0; i < _epoll_fds.size(); i++)
		if(_epoll_fds[i] == fd)
			return true;
	return false;
}


void Server::start()
{
	struct epoll_event events[EVENT_SIZE];
	int n_fds;
	std::cout << "Webserv started, awaiting for incoming connections" << std::endl;
	while (1)
	{
		if((n_fds = epoll_wait(_epoll_instance, events, EVENT_SIZE, -1)) == -1)
			throw std::runtime_error("Failed to register epoll events (epoll_wait");

		for (int i = 0; i < n_fds; i++)
		{
			int ev_fd = events[i].data.fd;
			int ev = events[i].events;

			if (ev & EPOLLIN)
			{
				if(is_registered_socket(ev_fd))
				{
					while(1)
					{
						/*
							accept() créer un nouvel fd pour chaque requete envoyé sur les ports (fd) qu'on monitor à l'init
							donc obligé de le monitor à nouveau avec epoll_ctl pour communiquer ensuite avec 
							+ le rendre également non bloquant avec fnctl (même routine que dans init sockets)
						*/

						struct sockaddr_in client_addr;
						socklen_t client_len = sizeof(client_addr);
						std::memset(&client_addr, 0, sizeof(client_addr));
						int accept_fd = accept(ev_fd, (struct sockaddr*)&client_addr, &client_len);

						if (accept_fd == -1)
						{
							if (errno == EAGAIN || errno == EWOULDBLOCK)
								break ; // Plus de connections à accepter
							else
								throw std::runtime_error("accept() syscall failed");
						}

						int flags = fcntl(accept_fd, F_GETFL, 0);
						fcntl(accept_fd, F_SETFL, flags | O_NONBLOCK);

						struct epoll_event accept_event;
						std::memset(&accept_event, 0, sizeof(accept_event));
						accept_event.events = EPOLLIN | EPOLLET;
						accept_event.data.fd = accept_fd;

						epoll_ctl(_epoll_instance, EPOLL_CTL_ADD, accept_fd, &accept_event);
					}
				}
				else
				{
					char buffer[1024];
					int rd = read(ev_fd, buffer, sizeof(buffer));
					if(rd == -1)
					{
						std::cout << "read failed for fd " << ev_fd << std::endl;
						//close(ev_fd);
						break ;
					}
					else if(rd == 0)
					{
						std::cout << "no more data to recieve" << std::endl;
						close(ev_fd);
						break ;
					}
					else
					{
						std::cout << "Received " << rd << " bytes" << std::endl << std::endl;
						std::cout.write(buffer, rd);
						std::cout << std::endl;
					}
				}
			}
		}
	}
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
