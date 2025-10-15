/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:03:09 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/15 17:05:38 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include <vector>

#include "Location.hpp"
#include "Return.hpp"

class Server
{
	private:
		class ParsServer;

	private:
		unsigned long	_fDefined;

		std::string		_host;
		std::string		_port;
		std::string		_url;

		std::vector<std::string>	_serverNames;
		std::map<std::string, std::string>	_errPages;

		unsigned long	_maxBodySize;

		std::vector<Location>	_locations;

	public:
		Server(void);
		Server(std::ifstream &configFile, std::string &line);
		~Server(void);
};

#endif
