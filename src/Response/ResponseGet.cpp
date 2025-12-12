/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseGet.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:12:22 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/10 12:15:32 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Client.hpp"
#include "FStat.hpp"
#include "ReadDir.hpp"

bool	Response::fileToBody(char const fName[])
{
	int fd = open(fName, O_RDONLY);
	if (fd < 0)
	{
		_responseCode = 403;
		return (false);
	}
	else
	{
		_body.clear();
		const int	toRead = 1024;		
		char		buffer[toRead];
		int			br(toRead);
		while (br)
		{
			br = read(fd, buffer, toRead);
			if (br < 0)
			{
				_responseCode = 500;
				return (false);
			}
			_body.append(buffer, br);
		}
		close(fd);
	}
	return (true);
}

bool	Response::lookForIndex(const char dName[])	// Returns true if one index was found
{
	typedef	std::vector<std::string>::const_iterator	VecStrConstIt;
	const std::vector<std::string> indexs = _location->getIndexs();

	FStat	fileStat;

	for (VecStrConstIt it = indexs.begin(); it != indexs.end(); ++it)
	{
		fileStat.open(dName, *it);
		if (fileStat.isFile())
		{
			fileToBody(fileStat.getPathCStr());
			return (true);
		}
	}
	return (false);
}

void	Response::_handleGET(void)
{
	FStat	fileStat(_location->getRoot(), _URI);
	if (fileStat.isFile())
		fileToBody(fileStat.getPathCStr());
	else if (fileStat.isDir())
	{
		if (!lookForIndex(fileStat.getPathCStr()))
		{
			if (_location->isAutoIndexOn())
			{
				generateAutoIndex(fileStat.getPathCStr());
			}
			else
				_responseCode = 404;
		}
	}
	else if (fileStat.fail())
		_responseCode = 404;
	else
		_responseCode = 403;
}
