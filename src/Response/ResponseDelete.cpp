/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseDelete.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:41:12 by gaeudes           #+#    #+#             */
/*   Updated: 2025/12/12 19:27:26 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Location.hpp"

void	Response::_handleDELETE(void)
{
	std::string	full_path = _location->getRoot() + '/' + _URI;
	full_path = Location::simplifyLocationPath(full_path);

	if (!Location::isLocationPathValid(full_path))
	{
		_responseCode = 403;
		return;
	}
	FStat	delFileStat(full_path);
	if (delFileStat.fail())
		_responseCode = 404;
	else if (delFileStat.isDir() || !delFileStat.isWritable())
		_responseCode = 403;
	else if (unlink(full_path.c_str()) == 0)
	{
		_responseCode = 204;
		std::cout << "\033[1;32mDELETE: " << full_path << " deleted\033[0m" << std::endl;
	}
	else
	{
		_responseCode = 500;
		std::cout << "\033[1;31mDELETE: Failed to delete " << full_path << "\033[0m" << std::endl;
	}
}
