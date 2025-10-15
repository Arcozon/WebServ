/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsLocation.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:33:11 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/15 15:34:47 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParsLocation.hpp"

bool	Location::ParsLocation::_isOnOff(const std::string &str)
{
	return (str == "on" || str == "off");
}

Location::ParsLocation::ParsLocation(void)
:	_defined(0),
	_allowed(GET_MASK(S_GET) | GET_MASK(S_POST) | GET_MASK(S_DELETE)),
	_autoindex(false)
{}

Location::ParsLocation::~ParsLocation(void)
{}

// Return true to continue false if out of scope, throws on error
bool	Location::ParsLocation::_addLine(const std::string &line)
{
	std::size_t	nTab = _getTabDepth(line);

	if (nTab == std::string::npos)
		return (true);
	if (nTab < _nTabLocation)
		return (false);
	if (nTab > _nTabLocation)
		throw (MyException("Too many tabs", MyException::ELVL_FATAL, line));
	// _callSetMember(line.);
	return (true);
}
