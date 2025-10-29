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

int Server::_stop_signal = 0;

void Server::sigHandler(int signum)
{
	if (signum == SIGINT || signum == SIGTERM)
	{
		std::cout << std::endl << "Recieved " << (signum == SIGINT ? "SIGINT" : "SIGTERM") << std::endl;
		_stop_signal = 1;
	}
}

void Server::initSockets(IpPort *config)
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
	addr.sin_addr.s_addr = INADDR_ANY;
	//std::cout << "SERVER: " << config->getHost() << ":" << config->getPort() << std::endl;
	addr.sin_port = htons(std::atoi(config->getPort().c_str()));
	if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		std::ostringstream oss;
		oss << "bind() syscall failed to listen on port: " << config->getPort();
		throw std::runtime_error(oss.str());
	}
	if (listen(fd, SOMAXCONN) == -1)
	{
		std::ostringstream oss;
		oss << "listen() syscall failed to accept incoming connections on port: " << config->getPort();
		throw std::runtime_error(oss.str());
	}
	_fd_config[fd] = config;
	this->_epoll_fds.push_back(fd); // fd qu'on va monitor avec epoll
	
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

bool Server::isServerSocket(int fd)
{
	for(size_t i = 0; i < _epoll_fds.size(); i++)
		if(_epoll_fds[i] == fd)
			return true;
	return false;
}

void Server::registerNewClient(int server_fd)
{
	 while (1)
	 {
		/*
			accept() créer un nouvel fd pour chaque requete envoyé sur les ports (fd) qu'on monitor à l'init
			donc obligé de le monitor à nouveau avec epoll_ctl pour communiquer ensuite avec
			+ le rendre également non bloquant avec fnctl (même routine que dans init sockets)
		*/

		struct sockaddr_in client_addr;
		socklen_t client_len = sizeof(client_addr);
		std::memset(&client_addr, 0, sizeof(client_addr));
		int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);

		if (client_fd == -1)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				return ;
			else
				throw std::runtime_error("accept() syscall failed");
		}

		int flags = fcntl(client_fd, F_GETFL, 0);
		fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);

		struct epoll_event accept_event;
		std::memset(&accept_event, 0, sizeof(accept_event));
		accept_event.events = EPOLLIN | EPOLLET;
		accept_event.data.fd = client_fd;

		Client *client = new Client(client_fd, getConfig(server_fd));
		_clients[client_fd] = client;

		if(epoll_ctl(_epoll_instance, EPOLL_CTL_ADD, client_fd, &accept_event) == -1)
			throw std::runtime_error("epoll_ctl() failed");

	}
}

/* à potentiellement changer, si la clé n'existe pas (close hasardeux quelque part) 
	Client *cl = _clients[client_fd]; rajoute une nouvelle entrée en C++98 */

void Server::readFromClient(int client_fd)
{
	Client *cl = _clients[client_fd];
	cl->readFromFd();
}

void Server::start()
{
	struct epoll_event events[4096];
	int n_fds;
	std::cout << "Webserv started, awaiting for incoming connections" << std::endl;
	while (_stop_signal != 1)
	{
		n_fds = epoll_wait(_epoll_instance, events, EVENT_SIZE, 1000);
		if (n_fds == -1)
		{
			if (errno == EINTR)
				continue ;
			throw std::runtime_error("Failed to register epoll events (epoll_wait");
		}
		for (int i = 0; i < n_fds; i++)
		{
			int ev_fd = events[i].data.fd;
			int ev = events[i].events;

			if (ev & (EPOLLERR | EPOLLHUP))
			{
				std::cout << "Error for fd: " << ev_fd << std::endl;
				if (!isServerSocket(ev_fd))
				{
					epoll_ctl(_epoll_instance, EPOLL_CTL_DEL, ev_fd, NULL);
					Client* cl = _clients[ev_fd];
					delete cl;
					_clients.erase(ev_fd);
				}
				continue ;
			}
			if (isServerSocket(ev_fd))
			{
				if(ev & EPOLLIN)
					registerNewClient(ev_fd);
			}
			else if (ev & EPOLLIN)
			{
				readFromClient(ev_fd);
			}
			else if (ev & EPOLLOUT)
			{
				
			}
		}
	}
	std::cout << "Bonne nuit!" << std::endl;
}


Server::Server(std::vector<IpPort> &servers): _server_configs(servers)
{
	// _ports.push_back(8080);
	// _ports.push_back(8181);
	// _ports.push_back(8083);
	for(size_t i = 0; i < _server_configs.size(); i++)
	{
		std::cout << "test" << std::endl;
		initSockets(&_server_configs[i]);
	}
	initEpoll();
	signal(SIGINT, Server::sigHandler);
	signal(SIGTERM, Server::sigHandler);

}

Server::~Server(void)
{
	for(size_t i = 0; i < _epoll_fds.size(); i++)
		close(_epoll_fds[i]);
	close(_epoll_instance);

	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		delete it->second;
	_clients.clear();
}

IpPort *Server::getConfig(int fd)
{
	std::map<int, IpPort*>::iterator it = _fd_config.find(fd);
	if(it != _fd_config.end())
		return it->second;
	return NULL;
}
