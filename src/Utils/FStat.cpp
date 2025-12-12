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
	_readable(false),
	_writable(false)
{}

FStat::FStat(const std::string &path)
:	_fType(ERR_STAT),
	_readable(false),
	_writable(false)
{
	this->open(path);
}

FStat::FStat(const std::string &path, const std::string &URI)
:	_fType(ERR_STAT),
	_readable(false),
	_writable(false)
{
	this->open(path, URI);
}

FStat::~FStat(void)
{}

bool	FStat::open(const std::string &path)
{
	struct stat	buffStat = {};

	_path = Location::simplifyLocationPath(path);
	_readable = false;
	_writable = false;
	_fType = ERR_STAT;
	if (stat(_path.c_str(), &buffStat) == 0)
	{
		_fType = UNKNOWN;
		if (S_ISDIR(buffStat.st_mode))
			_fType = DIRECTORY;
		else if (S_ISREG(buffStat.st_mode))
			_fType = REG_FILE;
		if (access(_path.c_str(), R_OK) == 0)
			_readable = true;
		if (access(_path.c_str(), W_OK) == 0)
			_writable = true;
	}
	return ( this->fail() );
}

bool	FStat::open(const std::string &path, const std::string &URI)
{
	_readable = false;
	_writable = false;
	_fType = ERR_STAT;
	{
		std::string catPath = path;

		if (!URI.empty())
			catPath += "/" + URI;
		catPath = Location::simplifyLocationPath(catPath);
		_path = catPath;
	}
	return ( this->open(_path) );
}

bool	FStat::fail(void) const
{
	return (_fType == ERR_STAT);
}

bool	FStat::isDir(void) const
{
	return (_fType == DIRECTORY);
}

bool	FStat::isFile(void) const
{
	return (_fType == REG_FILE);
}

bool	FStat::isReadable(void) const
{
	return (_readable);
}

bool	FStat::isWritable(void) const
{
	return (_writable);
}

const char	*FStat::getPathCStr(void) const
{
	return (_path.c_str());
}

