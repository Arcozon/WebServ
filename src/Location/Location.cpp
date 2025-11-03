/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:37:11 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/03 12:57:50 by gaeudes          ###   ########.fr       */
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
:	_valid(true),
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

const std::string	&Location::getLocation(void) const
{
	return (_location);
}

bool	Location::isMethodAllowed(const sAllowedMethods &method) const
{
	if (method >= s_METHODS_MAX)
		return (false);
	return (_flags & (1 << method));
}

bool	Location::isMethodAllowed(const std::string &method) const
{
	if (method == "GET")
		return (_flags & (1 << s_GET));
	else if (method == "POST")
		return (_flags & (1 << s_POST));
	else if (method == "DELETE")
		return (_flags & (1 << s_DELETE));
	return (false);
}

std::string	Location::simplifyPath(const std::string &basePath)
{
	static std::string	slashSlash("//");
	static std::string	slashDotSlash("/./");
	
	std::string	finalPath(basePath);

	std::size_t	iSlashSlash;
	do
	{
		iSlashSlash = finalPath.find(slashSlash);
		if (iSlashSlash != std::string::npos)
			finalPath.erase(iSlashSlash, slashSlash.size() - 1);
	}	while (iSlashSlash != std::string::npos);

	std::size_t	iSlashDotSlash;
	do
	{
		iSlashDotSlash = finalPath.find(slashDotSlash);
		if (iSlashDotSlash != std::string::npos)
			finalPath.erase(iSlashDotSlash, slashDotSlash.size() - 1);
	}	while (iSlashDotSlash != std::string::npos);

	return (finalPath);	
}
