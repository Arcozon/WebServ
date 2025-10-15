/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsWebServ.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:50:31 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/15 13:06:43 by gaeudes          ###   ########.fr       */
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
		std::ifstream		_configFile;

		std::string			_pname;
		std::vector<Server>	_servers; 

	private:
		void	_openConfigFile(const int ac, char *av[]);

	public:
		ParsWebServ(const int ac, char *av[]);
		~ParsWebServ(void);
};

#endif
