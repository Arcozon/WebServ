/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDir.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 17:06:50 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/03 17:56:58 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FileDir.hpp"
#include "Location.hpp"

FileDir::FileDir(const std::string &path, const std::string &URI)
:	_path(Location::simplifyLocationPath(path + "/" +URI)),
	_fType(UNKMOWN),
	_readable(false)
{
	struct stat	bufStat = {};

	if (stat(_path.c_str(), &bufStat) == 0)
	{
		mode_t	bufMode = bufStat.st_mode & S_IFMT;
		if (bufMode == S_IFDIR)
			_fType = DIRECTORY;
		else if (bufMode == S_IFREG)
			_fType = REG_FILE;
		if (_fType != UNKMOWN)
		{
			if (access(_path.c_str(), R_OK) == 0)
				_readable = true;
		}
	}
}

FileDir::~FileDir(void)
{}

bool	FileDir::fail(void) const
{
	return (_fType == UNKMOWN || !_readable);
}

bool	FileDir::isDir(void) const
{
	return (_readable && _fType == DIRECTORY);
}

bool	FileDir::isFile(void) const
{
	return (_readable && _fType == REG_FILE);
}

bool	FileDir::isAccesible(void) const
{
	return (_readable);
}

const char	*FileDir::getPathCStr(void) const
{
	return (_path.c_str());
}
