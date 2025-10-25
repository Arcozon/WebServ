/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:37:11 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/25 15:25:23 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include "ParsLocation.hpp"

Location::Location(void)
:	_valid(false),
	_flags(0),
	_autoindex(false)
{}

Location::Location(ParsLine &parsLine)
:	_valid(false),
	_flags(0),
	_autoindex(false)
{
	ParsLocation	parsLocation(parsLine);

	if (!parsLocation.isParsLocationValid())
	{
		_valid = false;
		return ;
	}
	
	_flags = parsLocation.getLocationFlags();
	_location = parsLocation.getLocation();
	_root = parsLocation.getRoot();
	_index = parsLocation.getIndex();
	_autoindex = parsLocation.getAutoIndex();
	_cgiHandler = parsLocation.getCGIHandler();
	_errPages = parsLocation.getErrPages();
	_return = parsLocation.getReturn();	
}

Location::~Location(void)
{}

bool	Location::isValid(void) const
{
	return (_valid);
}
