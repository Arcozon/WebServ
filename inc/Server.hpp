/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:03:09 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/22 17:23:36 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include <vector>
#include <stdexcept>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <cerrno>
#include <map>
#include <signal.h>
#include <csignal>

#include "Client.hpp"
#include "IpPort.hpp"
#include "Sessions.hpp"

#define EVENT_SIZE 100

class Client;
class Sessions;

class Server
{
	private:
		class ParsWebServ;

	private:
		std::vector<int> _epoll_fds;
		std::map<int, Client *> _clients;
		std::vector<IpPort>	_server_configs;
		std::map<int, IpPort*>	_fd_config;
		int	_epoll_instance;
		static int _stop_signal;
		Sessions _sessions;

		void initSockets(IpPort *config);
		void initEpoll();
	 	bool isServerSocket(int fd);
		void registerNewClient(int server_fd);
		void readFromClient(int client_fd);
		void writeToClient(int client_fd);
		static void sigHandler(int signum);
		const IpPort &getConfig(int fd);
		void removeClient(int client_fd);
		void checkTimeouts();

		Server(void);
		static Server *_serv;

	public:
		
		Server(int ac, char *av[]);
		~Server(void);
		void start(); // démarre l'event loop du serveur
		void stop(); // arrête l'event loop du serveur
	
		static void linkServer(Server *serv);
		static void closeServer(void);
};

#endif
