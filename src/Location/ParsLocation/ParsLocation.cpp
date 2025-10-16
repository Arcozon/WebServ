/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsLocation.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:33:11 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/16 12:14:07 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParsLocation.hpp"

const std::string	Location::ParsLocation::_keyRoot("root");
const std::string	Location::ParsLocation::_keyIndex("index");
const std::string	Location::ParsLocation::_keyAutoIndex("autoindex");
const std::string	Location::ParsLocation::_keyAllow("allow");
const std::string	Location::ParsLocation::_keyUploadLocation("upload_store");
const std::string	Location::ParsLocation::_keyReturn("return");
const std::string	Location::ParsLocation::_keyCGIHandler("cgi_handler");
const std::string	Location::ParsLocation::_keyErrorPage("error_page");

bool	Location::ParsLocation::_isOnOff(const std::string &str)
{
	return (str == "on" || str == "off");
}

bool	Location::ParsLocation::_isDefined(alreadyDefined toTest) const
{
	return ((_fDefined & GET_MASK(toTest)) != 0);
}

void	Location::ParsLocation::_addDefined(alreadyDefined toTest)
{
	_fDefined |= GET_MASK(toTest);
}

Location::ParsLocation::~ParsLocation(void)
{}

void	Location::ParsLocation::_addLocationLine(void)
{
	if (_getTabDepth(_line) > _nTabLocation)
		throw (MyException("Too many tabs", MyException::ELVL_ERROR, _line));
	
	_splitLine = _split(_line);
	if (_splitLine.front() ==_keyRoot)
		_addRoot();
	else if (_splitLine.front() ==_keyIndex)
		_addIndex();
	else if (_splitLine.front() ==_keyAutoIndex)
		_addAutoIndex();
	else if (_splitLine.front() ==_keyAllow)
		_addAllow();
	else if (_splitLine.front() ==_keyUploadLocation)
		_addUploadLocation();
	else if (_splitLine.front() ==_keyReturn)
		_addReturn();
	else if (_splitLine.front() ==_keyCGIHandler)
		_addCGIHandler();
	else if (_splitLine.front() ==_keyErrorPage)
		_addErrPages();
	else
		throw (MyException("Unknown key in location",
			MyException::ELVL_ERROR, _splitLine.front()));	
}

void	Location::ParsLocation::_addRoot(void)
{
	if (_isDefined(S_root))
		throw (MyException("Already defined", MyException::ELVL_ERROR, _splitLine.front()));

	_addDefined(S_root);
}

void	Location::ParsLocation::_addIndex(void)
{
	if (_splitLine.size() == 1)
		throw (MyException("Needs arguments", MyException::ELVL_ERROR, _splitLine.front()));
	for (std::vector<std::string>::size_type i = 1; i < _splitLine.size(); ++i)
		_index.push_back(_splitLine.at(i));
}

void	Location::ParsLocation::_addAutoIndex(void)
{
	if (_isDefined(S_autoindex))
		throw (MyException("Already defined", MyException::ELVL_ERROR, _splitLine.front()));
	else if (_splitLine.size() != 2)
		throw (MyException("Needs one argument", MyException::ELVL_ERROR, _splitLine.front()));
	else if (!_isOnOff(_splitLine.at(1)))
		throw (MyException("Needs to be [on] or [off]", MyException::ELVL_ERROR, _splitLine.at(1)));

	_autoindex = (_splitLine.at(1) == "on");
	_addDefined(S_autoindex);
}

void	Location::ParsLocation::_addAllow(void)
{
	if (_isDefined(S_allow))
		throw (MyException("Already defined", MyException::ELVL_ERROR, _splitLine.front()));
	else if (_splitLine.size() == 1)
		throw (MyException("Needs arguments", MyException::ELVL_ERROR, _splitLine.front()));
	_addDefined(S_allow);
}

void	Location::ParsLocation::_addUploadLocation(void)
{
	if (_isDefined(S_upload_store))
		throw (MyException("Already defined", MyException::ELVL_ERROR, _splitLine.front()));
	else if (_splitLine.size() != 2)
		throw (MyException("Needs one argument", MyException::ELVL_ERROR, _splitLine.front()));
	_uploadLocation = _splitLine.at(1);
	_addDefined(S_upload_store);
}

void	Location::ParsLocation::_addReturn(void)
{
	if (_isDefined(S_return))
		throw (MyException("Already defined", MyException::ELVL_ERROR, _splitLine.front()));
	else if (_splitLine.size() != 2 && _splitLine.size() != 3)
		throw (MyException("Needs one or two arguments", MyException::ELVL_ERROR, _splitLine.front()));
	// ADD return
	_addDefined(S_return);
}

void	Location::ParsLocation::_addCGIHandler(void)
{
	if (_splitLine.size() != 3)
		throw (MyException("Needs two arguments", MyException::ELVL_ERROR, _splitLine.front()));
	if (_splitLine[1].size() < 2 || _splitLine[1].at(0) != '.')
		throw (MyException("Not a valid file extension", MyException::ELVL_ERROR, _splitLine.at(1)));
	_cgiHandler.insert(std::pair<std::string, std::string>(_splitLine.at(1), _splitLine.at(2)));
}

void	Location::ParsLocation::_addErrPages(void)
{
	if (_splitLine.size() != 3)
		throw (MyException("Needs two arguments", MyException::ELVL_ERROR, _splitLine.front()));
	else if (!_isHTTPErrorCode(_splitLine[1]))
		throw (MyException("Not a valid HTTP error code", MyException::ELVL_ERROR, _splitLine.at(1)));
	else if (_errorPages.find(_splitLine[1]) != _errorPages.end())
		throw (MyException("Error page is already defined", MyException::ELVL_ERROR, _splitLine.at(1)));
	_errorPages.insert(std::pair<std::string, std::string>(_splitLine.at(1), _splitLine.at(2)));
}


Location::ParsLocation::ParsLocation(std::ifstream &configFile, std::string &line)
:	_configFile(configFile),
	_line(line),
	_fDefined(0),
	_autoindex(_defaultAutoIndex),
	_allow( GET_MASK(S_GET) | GET_MASK(S_POST) | GET_MASK(S_DELETE) )
{
	_splitLine = _split(_line);

	_location = _splitLine[1];
	while (std::getline(_configFile, _line))
	{
		if (_isLineEmpty(_line))
			continue ;
		else if (_getTabDepth(_line) < _nTabLocation)
			break ;
		try
		{
			_addLocationLine();
		}
		catch(const MyException& e)
		{
			e.throwDown();
			std::cerr << e;
		}			
	}
}
