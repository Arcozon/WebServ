/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 10:51:19 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/22 16:12:25 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include "CGIHandler.hpp"

#include "ParsLine.hpp"

int	main(int ac, char **av, char **env)
{
	{
		CGIHandler::copyEnv(env);
		CGIHandler::printEnv();
		CGIHandler::addVar("Salut", "toi");
		CGIHandler::printEnv();
	}
<<<<<<< HEAD
	// try
	// {
	// 	WebServ	MyWebServ(ac, av, env);
	// }
	// catch (const MyException &e)
	// {
	// 	std::cerr << e;
	// 	return (e.getErrLvl() + 1);
	// }
=======
	try
	{
		Server server;
		//server.print_fds();
		server.start();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
>>>>>>> origin/saal-kur_server
	return (EXIT_SUCCESS);
	(void)ac, (void)av, (void)env;
}
