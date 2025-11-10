/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseError.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 10:55:15 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/10 11:39:36 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "IpPort.hpp"
#include "FStat.hpp"

void	Response::_makeErrPage()
{}

void	Response::_handleError(void)
{
	if (_ipPort.errorPageDefined(_responseCode))
	{
		std::string	errPageFile = _ipPort.getErrorPage(_responseCode);
		FStat	fileStat(errPageFile);

		if (fileStat.isFile())
			fileToBody(fileStat.getPathCStr());
		else
			_makeErrPage();
	}
	else
	{
		_makeErrPage();
	}
}
