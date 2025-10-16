/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:50:09 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/16 14:47:31 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "ParsServer.hpp"

Server::Server(void)
:	_fDefined(0),
	_maxBodySize(0)
{}

Server::Server(ParsLine &parsLine)
:	_fDefined(0),
	_maxBodySize(0)
{
	ParsServer	parsServer(parsLine);

	(void)parsServer;
}

Server::~Server(void)
{}
