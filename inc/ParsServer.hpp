/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:25:55 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/16 14:49:08 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSSERVER_HPP
# define PARSSERVER_HPP

# include <fstream>
# include <string>
# include <vector>
# include <map>
# include <utility>

# include "Server.hpp"
# include "Location.hpp"
# include "MyException.hpp"

# include "ParsLine.hpp"
# include "utils.hpp"

class Server::ParsServer
{
	private:
		enum alreadyDefined
		{
			S_host,
			S_port,
			S_clientBodySize,
			S_return
		};

		static const std::size_t	_nTabServer = 1;

		static const std::string	_keyHost;
		static const std::string	_keyPort;
		static const std::string	_keyServerName;
		static const std::string	_keyClientBodySize;
		static const std::string	_keyErrorPage;
		static const std::string	_keyLocation;
		static const std::string	_keyReturn;

	private:
		ParsLine	&_parsLine;

		unsigned long	_fDefined;

		std::string							_host;
		std::string							_port;
		std::vector<std::string>			_serverNames;
		std::string							_clientMaxBodySize;
		std::map<std::string, std::string>	_errorPages;
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

		void	_addDefined(alreadyDefined toTest);
		bool	_isDefined(alreadyDefined toTest) const;

		void	printParsServ(void) const;

	public:
		ParsServer(ParsLine &parsLine);
		~ParsServer(void);

		bool	_isServValid(void) const;
};

#endif
