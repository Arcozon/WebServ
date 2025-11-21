/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseCGI.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 16:22:44 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/21 17:39:06 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "CGI.hpp"

// static void	_getContextCGI(const std::string &URI,
// 				std::string &scriptName,
// 				std::string &pathInfo)
// {
// 	std::string::size_type	startScriptName = URI.find_first_not_of('/'); 
// 	if (startScriptName == std::string::npos)
// 		startScriptName = 0;
// 	std::string::size_type	endScriptName = URI.find_first_of("/", startScriptName);
	
// 	scriptName = URI.substr(startScriptName, endScriptName - startScriptName);
	
// 	if (endScriptName != std::string::npos)
// 		pathInfo = URI.substr(endScriptName);
// }

static std::string	_getScriptName(const std::string &URI)
{
	std::string::size_type	startScriptName = URI.find_first_not_of('/'); 
	if (startScriptName == std::string::npos)
		startScriptName = 0;
	std::string::size_type	endScriptName = URI.find_first_of("/", startScriptName);
	
	return (URI.substr(startScriptName, endScriptName - startScriptName));
}

void	Response::_handleCGI(void)
{
	// std::string scriptName, pathInfo;

	// _getContextCGI(_URI, scriptName, pathInfo);
	if (_location->_hasCGIHandler(_getScriptName(_URI)))
	{
		_isCGI = true;
		const std::string	&bin = _location->getCgiHandler(_getScriptName(_URI));
		// CGI	handler(bin, _location->getRoot(), scriptName, pathInfo, _cl->getQueryString(), _cl->getMethod(),
		// 	_cl->getHeader(), _ipPort, _location, _body);
		CGI	handler(bin, _location->getRoot(), _URI, _cl->getQueryString(), _cl->getMethod(),
			_cl->getHeader(), _ipPort, _location, _body);
		
		handler.forkCGI();
		if (!handler.fail())
		{
			while (!handler.isDone())
			{
				;
			}
			int fd = handler.getReadPipe();
			int br;
			char	buff[1024];
			_body.clear();
			do
			{
				br = read(fd, buff, sizeof(1024));
				if (br > 0)
				_body.append(buff, br);
			}	while (br > 0);
		}
		_responseCode = handler.getResponseCode();
		if (isErrorCode(_responseCode))
			_isCGI = false;
	}
}
