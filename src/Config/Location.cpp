/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:37:11 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/22 17:06:52 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include "ParsLocation.hpp"

Location::Location(void)
:	_valid(false),
	_flags(0)
{}

Location::Location(ParsLine &parsLine)
:	_valid(true),
	_flags(0)
{
	ParsLocation	parsLocation(parsLine);

	if (!parsLocation.isParsLocationValid())
	{
		_valid = false;
		return ;
	}
	
	_flags = parsLocation.getLocationFlags();
	_location = parsLocation.getLocation();
	_root = parsLocation.getRoot();
	_index = parsLocation.getIndex();
	_cgiHandler = parsLocation.getCGIHandler();
	_errPages = parsLocation.getErrPages();
	_return = parsLocation.getReturn();
	_uploadLocation = parsLocation.getUploadLocation();
}

Location::~Location(void)
{}

bool	Location::isValid(void) const
{
	return (_valid);
}

bool	Location::isMethodAllowed(const sAllowedMethods &methodCode) const
{
	if (methodCode >= s_METHODS_MAX)
		return (false);
	return (_flags & (1 << methodCode));
}

bool	Location::isMethodAllowed(const std::string &method) const
{
	sAllowedMethods methodCode = getMethodCode(method);

	return (methodCode);
}

bool	Location::isRootDefined(void)	const	{	return (_flags & GET_MASK(s_root));	}
bool	Location::isIndexDefined(void)	const	{	return (_index.size() != 0);	}
bool	Location::isAutoIndexOn(void)	const	{	return (_flags & GET_MASK(s_autoindex));	}
bool	Location::isUploadDefined(void)	const	{	return (_flags & GET_MASK(s_upload_store));	}
bool	Location::isCGIDefined(void)	const	{	return (_cgiHandler.size() != 0);	}
bool	Location::isReturnDefined(void)	const	{	return (_flags & GET_MASK(s_return));	}

const std::string	&Location::getLocation(void) const
{	return (_location);	}

const std::string	&Location::getRoot(void) const
{	return (_root);	}

const std::vector<std::string>	&Location::getIndexs(void) const
{	return (_index);	}

const std::string	&Location::getUploadLocation(void) const
{	return (_uploadLocation);	}

const std::map<std::string, std::string>	&Location::getCgiHandler(void) const
{	return (_cgiHandler);	}

const std::string	&Location::getCgiHandler(const std::string &file) const
{
	typedef std::map<std::string, std::string>::const_iterator MapStrStrConstIt;
	std::string	fExtension;

	std::string::size_type	lastDot = file.rfind('.');
	if (lastDot == std::string::npos)
		lastDot = 0;
	fExtension = file.substr(lastDot);
	
	MapStrStrConstIt itHandler = _cgiHandler.find(fExtension);

	return (itHandler->second);
}

bool	Location::_hasCGIHandler(const std::string &file) const
{
	std::string	fExtension;
	std::size_t	iDot = 0;

	for (std::size_t iSlash = 0; iSlash < file.size(); )
	{
		iDot = file.find('.', iSlash);
		if (iDot != std::string::npos)
		{
			iSlash = file.find('/', iDot);
			fExtension = file.substr(iDot, iSlash - iDot);
			if (fExtension.size() == 1)
				continue ;
			else if (_cgiHandler.find(fExtension) != _cgiHandler.end())
				return (true); 
		}
		else
			break;
	}
	return (false);	
}

const Return		&Location::getReturn(void) const
{	return (_return);	}

std::string	Location::simplifyLocationPath(const std::string &basePath)
{
	static std::string	slashSlash("//");
	static std::string	slashDotSlash("/./");
	
	std::string	finalPath(basePath);

	std::size_t	iSlashSlash;
	do
	{
		iSlashSlash = finalPath.find(slashSlash);
		if (iSlashSlash != std::string::npos)
			finalPath.erase(iSlashSlash, slashSlash.size() - 1);
	}	while (iSlashSlash != std::string::npos);

	std::size_t	iSlashDotSlash;
	do
	{
		iSlashDotSlash = finalPath.find(slashDotSlash);
		if (iSlashDotSlash != std::string::npos)
			finalPath.erase(iSlashDotSlash, slashDotSlash.size() - 1);
	}	while (iSlashDotSlash != std::string::npos);
	if (finalPath.find("./") == 0)
		finalPath.erase(0, 2);
	return (finalPath);	
}

bool	Location::isLocationPathValid(const std::string &path)
{
	if (path.empty() || path.find("/../") != std::string::npos || path.find("../") == 0)
		return (false);

	const std::size_t	indexSlashDotDot = path.rfind("/..");
	if (indexSlashDotDot != std::string::npos && indexSlashDotDot == path.size() - 3)
		return (false);
	return (true);
}

Location::sAllowedMethods	Location::getMethodCode(const std::string &method)
{
	if (method == "GET")
		return (s_GET);
	else if (method == "POST")
		return (s_POST);
	else if (method == "DELETE")
		return (s_DELETE);
	return (s_METHODS_MAX);
}

void	Location::fixReturn(const std::string &domainName)
{
	_return._fixUrlWithDomainName(domainName);
}
