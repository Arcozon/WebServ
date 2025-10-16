/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsWebServ.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:50:31 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/16 14:46:20 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSWEBSERV_HPP
# define PARSWEBSERV_HPP

# include <fstream>

# include <string>
# include <vector>

# include "MyException.hpp"
# include "WebServ.hpp"

# include "ParsLine.hpp"

class WebServ::ParsWebServ
{
	private:
		static const std::string	_keyServer;
	
	private:
		ParsLine	_parsLine;

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
