/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseError.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 10:55:15 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/22 19:01:58 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "MakeHTML.hpp"

#include "IpPort.hpp"
#include "FStat.hpp"

void	Response::_makeErrPage()
{
	MakeHTML	maker(_body);
	std::string	errMsg;

	maker.addMarkup("html");
	{
		errMsg = getReasonPhrase(_responseCode);
		maker.addMarkupWithText("title", "Error: " + errMsg);
	}
	maker.addMarkup("body");
	maker.addMarkup("h1");
	{
		std::stringstream errStr;
		errStr << _responseCode;
		maker.addText(errStr.str() + " " + errMsg);
	}
}

void	Response::_handleError(void)
{
	// TODO: Check if code is an err ?
	if (_ipPort.errorPageDefined(_responseCode))
	{
		std::string	errPageFile = _ipPort.getErrorPage(_responseCode);
		FStat	fileStat(errPageFile);

		if (fileStat.isFile() && fileStat.isReadable())
		{
			// std::cout << fileStat.getPathCStr() << std::endl; 
			fileToBody(fileStat.getPathCStr());
		}
		else
			_makeErrPage();
	}
	else
	{
		_makeErrPage();
	}
}
