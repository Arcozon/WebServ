/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IpPort.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:03:09 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/19 11:14:57 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IPPORT_HPP
# define IPPORT_HPP

<<<<<<< HEAD:inc/Server.hpp
#include <string>
#include <vector>
#include <stdexcept>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
<<<<<<< HEAD:inc/IpPort.hpp
=======
# include <string>
# include <vector>
=======
#include <cstring>
#include <sstream>
#include <cerrno>
#include <map>
#include "Client.hpp"

#define EVENT_SIZE 100

class Client;
>>>>>>> origin/saal-kur_server:inc/Server.hpp

# include "Location.hpp"
# include "Return.hpp"
>>>>>>> gaeudes_pars_config:inc/IpPort.hpp

# include "ParsLine.hpp"

class IpPort
{
	private:
<<<<<<< HEAD:inc/Server.hpp
		std::vector<unsigned short> _ports; 
		std::vector<int> _epoll_fds; 
		std::map<int, Client *> _clients;
		int	_epoll_instance;
=======
		class ParsIpPort;

	private:
		unsigned long	_fDefined;

		std::string		_host;
		std::string		_port;
		std::string		_url;

		std::vector<std::string>			_IpPortNames;
		std::map<std::string, std::string>	_errPages;

		unsigned long	_maxBodySize;

		std::vector<Location>	_locations;
>>>>>>> gaeudes_pars_config:inc/IpPort.hpp

		void initSockets();
		void initEpoll();
<<<<<<< HEAD:inc/IpPort.hpp
	public:
<<<<<<< HEAD:inc/Server.hpp
=======
	 	bool isServerSocket(int fd);
		void registerNewClient(int client_fd);
		void readFromClient(int client_fd);

		public:
>>>>>>> origin/saal-kur_server:inc/Server.hpp
		Server(void);
		~Server(void);
		void start(); // démarre l'event loop du serveur
		void stop(); // arrête l'event loop du serveur
=======
		IpPort(void);
		IpPort(ParsLine &parsLine);
		~IpPort(void);
>>>>>>> gaeudes_pars_config:inc/IpPort.hpp
};

#endif
