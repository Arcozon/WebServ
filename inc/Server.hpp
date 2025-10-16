/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:03:09 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/14 18:03:52 by gaeudes          ###   ########.fr       */
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

#define EVENT_SIZE 100

class Server
{
	private:
		std::vector<unsigned short> _ports; 
		std::vector<int> _epoll_fds; 
		int	_epoll_instance;

		void initSockets();
		void initEpoll();
	 	bool is_registered_socket(int fd);
		
		public:
		Server(void);
		~Server(void);
		void start(); // démarre l'event loop du serveur
		void stop(); // arrête l'event loop du serveur
		void print_fds();
};

#endif
