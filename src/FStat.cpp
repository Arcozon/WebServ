/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FStat.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 17:06:50 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/04 13:04:41 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FStat.hpp"
#include "Location.hpp"

FStat::FStat(void)
:	_fType(ERR_STAT),
	_readable(false)
{}

FStat::FStat(const std::string &path, const std::string &URI)
:	_fType(ERR_STAT),
	_readable(false)
{
	this->open(path, URI);
}

FStat::~FStat(void)
{}

bool	FStat::open(const std::string &path, const std::string &URI)
{
	struct stat	bufStat = {};

	_path = Location::simplifyLocationPath(path + "/" +URI);
	_fType = ERR_STAT;
	_readable = false;
	if (stat(_path.c_str(), &bufStat) == 0)
	{
		_fType = UNKNOWN;
		mode_t	bufMode = bufStat.st_mode & S_IFMT;
		if (bufMode == S_IFDIR)
			_fType = DIRECTORY;
		else if (bufMode == S_IFREG)
			_fType = REG_FILE;
		if (access(_path.c_str(), R_OK) == 0)
			_readable = true;
	}
	return ( this->fail() );
}

bool	FStat::fail(void) const
{
	return (_fType == ERR_STAT);
}

bool	FStat::isDir(void) const
{
	return (_readable && _fType == DIRECTORY);
}

bool	FStat::isFile(void) const
{
	return (_readable && _fType == REG_FILE);
}

bool	FStat::isReadable(void) const
{
	return (_readable);
}

const char	*FStat::getPathCStr(void) const
{
	return (_path.c_str());
}
