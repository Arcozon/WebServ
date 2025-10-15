/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 10:51:19 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/15 15:58:58 by gaeudes          ###   ########.fr       */
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
		return (e.getErrLvl() + 1);
	}
	return (EXIT_SUCCESS);
}
