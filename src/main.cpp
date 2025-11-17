/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 10:51:19 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/17 12:05:49 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include "CGIHandler.hpp"

#include "ParsLine.hpp"
#include "Server.hpp"

int	main(int ac, char **av, char **env)
{
	// {
	// 	CGIHandler::copyEnv(env);
	// 	CGIHandler::printEnv();
	// 	CGIHandler::addVar("Salut", "toi");
	// 	CGIHandler::printEnv();
	// }
	// try
	// {
	// 	WebServ	MyWebServ(ac, av, env);
	// }
	// catch (const MyException &e)
	// {
	// 	std::cerr << e;
	// 	return (e.getErrLvl() + 1);
	// }
	// try
	// {
	// 	WebServ	MyWebServ(ac, av, env);
	// }
	// catch (const MyException &e)
	// {
	// 	std::cerr << e;
	// 	return (e.getErrLvl() + 1);
	// }
	try
	{
		WebServ	MyWebServ(ac, av, env);
		Server server(MyWebServ.getServers());
		//server.print_fds();
		// server.start();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	return (EXIT_SUCCESS);
	(void)ac, (void)av, (void)env;
}
