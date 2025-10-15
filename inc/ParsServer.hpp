/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:25:55 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/15 14:51:20 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSSERVER_HPP
# define PARSSERVER_HPP

# include "Server.hpp"
# include "MyException.hpp"

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
	private:
		std::string	_host;
		std::string	_port;
		std::vector<std::string>			server_names;
		std::string							_client_max_body_size;
		std::map<std::string, std::string>	_error_pages;
		std::vector<Location>				_locations;
		Return								_return;
	public:
		ParsServer(void);
		~ParsServer(void);
};

#endif
