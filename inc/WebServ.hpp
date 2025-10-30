/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 10:44:46 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/30 11:23:24 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>

# include <string>
# include <vector>

# include <unistd.h>
# include <cstdlib>

# include "MyException.hpp"
# include "IpPort.hpp"

# define DOT_CONFIG ".config"

class WebServ
{
	private:
		class ParsWebServ;

	private:
		std::vector<IpPort>	_ipPorts;

		std::string			_pname;
		char				**_env;
		bool				_init;

	private:
		
	public:
		WebServ(const int ac, char *av[], char *env[]);
		~WebServ(void);
		std::vector<IpPort> &getServers();

};


#endif
