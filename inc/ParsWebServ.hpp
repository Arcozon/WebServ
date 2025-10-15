/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsWebServ.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:50:31 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/15 17:04:03 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSWEBSERV_HPP
# define PARSWEBSERV_HPP

# include <fstream>

# include <string>
# include <vector>

# include "MyException.hpp"
# include "WebServ.hpp"

class WebServ::ParsWebServ
{
	private:
		static const std::string	_keyServer;
	
	private:
		std::ifstream	_configFile;
		std::string		_line;

		std::string			_pname;
		std::vector<Server>	_servers; 

	private:
		void	_openConfigFile(const int ac, char *av[]);
		void	_readConfigFile(void);

		void	_addServer(void);

	public:
		ParsWebServ(const int ac, char *av[]);
		~ParsWebServ(void);
};

#endif
