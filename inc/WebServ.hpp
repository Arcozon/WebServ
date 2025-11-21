/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 10:44:46 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/21 14:46:02 by gaeudes          ###   ########.fr       */
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
		static std::vector<IpPort>	_ipPorts;

		static std::string			_pname;
		static bool				_init;

	private:
		
	public:
		static void	initWebServ(const int ac, char *av[]);
		static void closeWebServ(void);
		static std::vector<IpPort> &getServers();
};

#endif
