/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:01:27 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/14 18:06:11 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

WebServ::~WebServ(void)
{}

std::vector<IpPort> &WebServ::getServers()
{
	return _servers;
}
