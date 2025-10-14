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

#include "Location.hpp"

class Server
{
	private:
		class ParsServer;

	private:
		std::vector<std::string>	_serverNames;
		std::string _ip;
		std::string _port;
		std::string _url;
		std::map<std::string, std::string>	_errPages;

		unsigned long	_maxBodySize;
		bool			_maxBodySizeDefined;

		std::vector<Location>	_locations;

	public:
		Server(void);
		~Server(void);
};

#endif
