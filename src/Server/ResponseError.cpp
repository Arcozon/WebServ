/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseError.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 10:55:15 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/10 12:43:48 by gaeudes          ###   ########.fr       */
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
		if (_responseCode == 200)
			errMsg = "OK";
		else if (_responseCode == 201)
			errMsg = "Created";
		else if (_responseCode == 204)
			errMsg = "No Content";
		else if (_responseCode == 301)
			errMsg = "Moved Permanently";
		else if (_responseCode == 400)
			errMsg = "Bad Request";
		else if (_responseCode == 403)
			errMsg = "Forbiden Access";
		else if (_responseCode == 404)
			errMsg = "Not Found";
		else if (_responseCode == 405)
			errMsg = "Method Not Allowed";
		else if (_responseCode == 500)
			errMsg = "Internal Server Error";
		else
			errMsg = "WTF";
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
	if (_ipPort.errorPageDefined(_responseCode))
	{
		std::string	errPageFile = _ipPort.getErrorPage(_responseCode);
		FStat	fileStat(errPageFile);

		std::cout << "defined " << _responseCode << std::endl;
		if (fileStat.isFile() && fileStat.isReadable())
		{
			std::cout << fileStat.getPathCStr() << std::endl; 
			fileToBody(fileStat.getPathCStr());
		}
		else
			_makeErrPage();
	}
	else
	{
		std::cout << "not defined " << _responseCode << std::endl;
		_makeErrPage();
	}
}
