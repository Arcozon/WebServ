/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponsePost.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:29:42 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/17 17:03:22 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Location.hpp"
#include "Client.hpp"

#include "CGI.hpp"

void	Response::_handlePOST(void)
{
	_handleCGI();
}

static void	_getContextCGI(const std::string &URI,
				std::string &scriptName,
				std::string &pathInfo,
				std::string &queryString)
{
	std::string::size_type	startScriptName = URI.find_first_not_of('/'); 
	if (startScriptName == std::string::npos)
		startScriptName = 0;
	std::string::size_type	endScriptName = URI.find_first_of("/?", startScriptName);
	if (endScriptName == std::string::npos)
		endScriptName = URI.size();
	std::string::size_type	endPathInfo = URI.find('?', endScriptName); 
	if (endPathInfo == std::string::npos)
		endPathInfo = URI.size();
	std::string::size_type	startQueryString = endPathInfo + (endPathInfo != URI.size());

	scriptName = URI.substr(startScriptName, endScriptName - startScriptName);
	pathInfo = URI.substr(endScriptName, endPathInfo - endScriptName);
	queryString = URI.substr(startQueryString);
}

void	Response::_handleCGI(void)
{
	std::cout << "URL: " << _cl->getTargetLocation() << '\n';
	std::cout << "LOC: " << _location->getLocation() << '\n';
	std::cout << "URI: " << this->_URI << '\n';

	std::string scriptName, pathInfo, queryString;

	_getContextCGI(_URI, scriptName, pathInfo, queryString);
	std::cout << "Script: " << scriptName << '\n';
	std::cout << "PathInfo: " << pathInfo << '\n';
	std::cout << "QueryString: " << queryString << '\n';
	if (_location->_hasCGIHandler(this->_URI))
	{
		std::string	CGIh = _location->getCgiHandler(scriptName);
		std::cout << "Handler: " << CGIh << '\n';
		CGI	handler(_location, _cl, _URI);
	}
}

