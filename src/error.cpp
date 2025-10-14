/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 11:06:47 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/14 11:38:14 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

#define DOT_CONFIG ".config"

bool	_is_dot_config(const std::string fname)
{
	const std::string	dot_config(DOT_CONFIG);
	
	if (dot_config.size() >= fname.size())
		return (false);
	return (fname.compare(fname.size() - dot_config.size(), dot_config.size(), dot_config) == 0);
}

const std::string	ft_basename(const std::string av0)
{
	if (av0.find_last_of('/') != std::string::npos)
		return (av0.substr(av0.find_last_of('/') + 1));
	return (av0);
}

bool	check_args(int ac, char  **av)
{
	if (ac == 0)
	{
		std::cerr << CBOLD CRED << "Error: argc == 0" << CRESET << std::endl;
		return (false);
	}
	if (ac != 2 || !_is_dot_config(av[1]))
	{
		const std::string basename_av0(ft_basename(av[0]));

		std::cerr << CBOLD CRED;
		std::cerr << basename_av0 << ": ";
		std::cerr << CNONBOLD << "error: ";
		if (ac < 2)
			std::cerr << "missing config file";
		else if (ac > 2)
			std::cerr << "too many args";
		else
			std::cerr << "file must end with " DOT_CONFIG;
		if (ac != 2)
		{
			std::cerr << "\n	Usage: " CRESET;
			std::cerr << "./" << basename_av0;
			std::cerr << " [FILE]" DOT_CONFIG; 
		}
		std::cerr << CRESET << std::endl;
		return (false);
	}
	return (true);
}
