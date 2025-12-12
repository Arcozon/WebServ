/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReadDir.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 13:15:59 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/04 14:31:34 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ReadDir.hpp"

ReadDir::ReadDir(const std::string str)
:	_dir(opendir(str.c_str())),
	_dirEnt(NULL)
{}

ReadDir::ReadDir(const char cStr[])
:	_dir(opendir(cStr)),
	_dirEnt(NULL)
{}

ReadDir::~ReadDir(void)
{
	if (_dir)
		closedir(_dir);
}

bool	ReadDir::fail(void) const
{
	return (_dir == NULL);
}

std::string	ReadDir::getNextFile(void)
{
	do
	{
		_dirEnt = readdir(_dir);
	}	while (_dirEnt &&
			(std::string(_dirEnt->d_name) == ".")); // || std::string(_dirEnt->d_name) == ".."));
	if (_dirEnt)
		return (_dirEnt->d_name);
	return ("");
}

FStat::FILETYPE	ReadDir::getFileType(void) const
{
	#ifdef _DIRENT_HAVE_D_TYPE
		if (_dirEnt->d_type == DT_DIR)
			return (FStat::DIRECTORY);
		else if (_dirEnt->d_type == DT_REG)
			return (FStat::REG_FILE);
	#endif
	return (FStat::UNKNOWN);
}
