/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:25:55 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/15 17:28:54 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSSERVER_HPP
# define PARSSERVER_HPP

# include <fstream>
# include <string>
# include <vector>
# include <map>

# include "Server.hpp"
# include "MyException.hpp"

# include "utils.hpp"

class Server::ParsServer
{
	private:
		enum alreadyDefined
		{
			S_host,
			S_port,
			S_client_body_size,
			S_return
		};

		static const std::size_t	_nTabServer = 2;

		static const std::string	_keyHost;
		static const std::string	_keyPort;
		static const std::string	_keyServerName;
		static const std::string	_keyClientBodySize;
		static const std::string	_keyErrorPage;
		static const std::string	_keyLocation;
		static const std::string	_keyReturn;

	private:
		std::ifstream				&_configFile;
		std::string					&_line;
		std::vector<std::string>	_splitLine;

		std::string	_host;
		std::string	_port;
		std::vector<std::string>			server_names;
		std::string							_client_max_body_size;
		std::map<std::string, std::string>	_error_pages;
		std::vector<Location>				_locations;
		Return								_return;

	private:
		void	_addServerLine(void);
		void	_addHost(void);
		void	_addPort(void);
		void	_addServerName(void);
		void	_addClientBodySize(void);
		void	_addErrorPage(void);
		void	_addLocation(void);
		void	_addReturn(void);

	public:
		ParsServer(std::ifstream &_configFile, std::string &line);
		~ParsServer(void);

		bool	_isServValid(void) const;
};

#endif
