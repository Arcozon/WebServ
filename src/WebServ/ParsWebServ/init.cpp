/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:09:45 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/14 18:10:03 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include "ParsWebServ.hpp"

WebServ::WebServ(const int ac, char *av[], char *env[])
:	_env(env), _init(false)
{
	ParsWebServ	pars(ac, av);
	
	_init = true;
}
