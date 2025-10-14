/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsWebServ.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:50:31 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/14 18:11:37 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSWEBSERV_HPP
# define PARSWEBSERV_HPP

# include "WebServ.hpp"
# include "MyException.hpp"

# include <fstream>

class WebServ::ParsWebServ
{
	private:
		std::ifstream		_configFile;

		std::string			_pname;

	private:
		void	_openConfigFile(const int ac, char *av[]);

	public:
		ParsWebServ(const int ac, char *av[]);
		~ParsWebServ(void);
};

#endif
