/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 10:51:19 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/14 13:37:08 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

int	main(int ac, char **av, char **env)
{
	try
	{
		WebServ	MyWebServ(ac, av, env);
	}
	catch (const MyException &e)
	{
		std::cerr << e;
		return (1);
	}
	return (0);
}
