/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsWebServ.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:50:31 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/30 11:21:39 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSWEBSERV_HPP
# define PARSWEBSERV_HPP

# include <iostream>
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
		std::vector<IpPort>	_ipPorts; 

	private:
		void	_openConfigFile(const int ac, char *av[]);
		void	_readConfigFile(void);

		void	_addServer(void);

	public:
		ParsWebServ(const int ac, char *av[]);
		const std::vector<IpPort>	getIpPorts(void) const;
		~ParsWebServ(void);
};

#endif
