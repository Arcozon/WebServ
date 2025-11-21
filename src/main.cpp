/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 10:51:19 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/21 14:53:13 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include "CGI.hpp"
#include "CGIEnv.hpp"

#include "ParsLine.hpp"
#include "Server.hpp"

int	main(int ac, char **av, char **env)
{
	try
	{
		CGI::CGIEnv::copyEnv(env);

		Server server(ac, av);
		// server.print_fds();
		server.start();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	return (EXIT_SUCCESS);
	(void)ac, (void)av, (void)env;
}
