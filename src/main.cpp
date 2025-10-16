/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 10:51:19 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/16 14:25:03 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

#include "ParsLine.hpp"

int	main(int ac, char **av, char **env)
{
	try
	{
		// WebServ	MyWebServ(ac, av, env);
		ParsLine	test(av[1]);
		while (test.readLine())
		{
			std::cout << test.getLine() << std::endl;
		}
	}
	catch (const MyException &e)
	{
		std::cerr << e;
		return (e.getErrLvl() + 1);
	}
	return (EXIT_SUCCESS);
	(void)ac, (void)av, (void)env;
}
