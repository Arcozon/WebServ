/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 10:44:46 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/14 13:24:44 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <fstream>

# include <string>
# include <vector>

# include "MyException.hpp"
# include "Server.hpp"

# define DOT_CONFIG ".config"

class WebServ
{
	private:
		std::vector<Server>	_servers;
		std::string			_pname;
		bool				_init;
		std::ifstream		_configFile;

	private:
		inline void	_openConfigFile(const int ac, char *av[]);

	public:
		WebServ(const int ac, char *av[], char *env[]);
		~WebServ(void);
};

#endif
