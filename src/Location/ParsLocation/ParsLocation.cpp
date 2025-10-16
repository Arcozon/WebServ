/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsLocation.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:33:11 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/16 16:40:09 by gaeudes          ###   ########.fr       */
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

bool	Location::ParsLocation::_isAllowed(allowMethods toTest) const
{
	return ((_allow & GET_MASK(toTest)) > 1);
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
	if (_parsLine.getTabDepth() > _nTabLocation)
		throw (MyException("Too many tabs", MyException::ELVL_ERROR, _parsLine.getLine()));
	
	const std::string	&splitLineFront( _parsLine.getSplitLine().front() );

	if (splitLineFront ==_keyRoot)
		_addRoot();
	else if (splitLineFront ==_keyIndex)
		_addIndex();
	else if (splitLineFront ==_keyAutoIndex)
		_addAutoIndex();
	else if (splitLineFront ==_keyAllow)
		_addAllow();
	else if (splitLineFront ==_keyUploadLocation)
		_addUploadLocation();
	else if (splitLineFront ==_keyReturn)
		_addReturn();
	else if (splitLineFront ==_keyCGIHandler)
		_addCGIHandler();
	else if (splitLineFront ==_keyErrorPage)
		_addErrPages();
	else
		throw (MyException("Unknown key" + _inLocation(),
			MyException::ELVL_ERROR, splitLineFront));	
}

void	Location::ParsLocation::_addRoot(void)
{
	const std::vector<std::string>	&splitLine( _parsLine.getSplitLine() );

	if (_isDefined(S_root))
		throw (MyException("Already defined"  + _inLocation(), MyException::ELVL_WARNING, splitLine.front()));
	_root = splitLine.at(1);
	_addDefined(S_root);
}

void	Location::ParsLocation::_addIndex(void)
{
	const std::vector<std::string>	&splitLine( _parsLine.getSplitLine() );

	if (splitLine.size() == 1)
		throw (MyException("Needs arguments", MyException::ELVL_WARNING, splitLine.front()));
	for (std::vector<std::string>::size_type i = 1; i < splitLine.size(); ++i)
		_index.push_back(splitLine.at(i));
}

void	Location::ParsLocation::_addAutoIndex(void)
{
	const std::vector<std::string>	&splitLine( _parsLine.getSplitLine() );

	if (_isDefined(S_autoindex))
		throw (MyException("Already defined" + _inLocation(), MyException::ELVL_WARNING, splitLine.front()));
	else if (splitLine.size() != 2)
		throw (MyException("Needs one argument", MyException::ELVL_WARNING, splitLine.front()));
	else if (!_isOnOff(splitLine.at(1)))
		throw (MyException("Needs to be [on] or [off]", MyException::ELVL_WARNING, splitLine.at(1)));

	_autoindex = (splitLine.at(1) == "on");
	_addDefined(S_autoindex);
}

void	Location::ParsLocation::_addAllow(void)
{
	const std::vector<std::string>	&splitLine( _parsLine.getSplitLine() );

	if (_isDefined(S_allow))
		throw (MyException("Already defined" + _inLocation(), MyException::ELVL_WARNING, splitLine.front()));
	else if (splitLine.size() == 1)
		throw (MyException("Needs arguments", MyException::ELVL_WARNING, splitLine.front()));
	_allow = 0;
	// add allow
	_addDefined(S_allow);
}

void	Location::ParsLocation::_addUploadLocation(void)
{
	const std::vector<std::string>	&splitLine( _parsLine.getSplitLine() );

	if (_isDefined(S_upload_store))
		throw (MyException("Already defined" + _inLocation(), MyException::ELVL_WARNING, splitLine.front()));
	else if (splitLine.size() != 2)
		throw (MyException("Needs one argument", MyException::ELVL_WARNING, splitLine.front()));
	_uploadLocation = splitLine.at(1);
	_addDefined(S_upload_store);
}

void	Location::ParsLocation::_addReturn(void)
{
	const std::vector<std::string>	&splitLine( _parsLine.getSplitLine() );

	if (_isDefined(S_return))
		throw (MyException("Already defined" + _inLocation(), MyException::ELVL_WARNING, splitLine.front()));
	else if (splitLine.size() != 2 && splitLine.size() != 3)
		throw (MyException("Needs one or two arguments", MyException::ELVL_WARNING, splitLine.front()));
	else if (splitLine.size() == 2)
		_return = Return(splitLine.at(1));
	else if (splitLine.size() == 3) 
		_return = Return(splitLine.at(1), splitLine.at(2));
	_addDefined(S_return);
}

void	Location::ParsLocation::_addCGIHandler(void)
{
	const std::vector<std::string>	&splitLine( _parsLine.getSplitLine() );

	if (splitLine.size() != 3)
		throw (MyException("Needs two arguments", MyException::ELVL_WARNING, splitLine.front()));
	if (splitLine[1].size() < 2 || splitLine[1].at(0) != '.')
		throw (MyException("Not a valid file extension", MyException::ELVL_WARNING, splitLine.at(1)));
	_cgiHandler.insert(std::pair<std::string, std::string>(splitLine.at(1), splitLine.at(2)));
}

void	Location::ParsLocation::_addErrPages(void)
{
	const std::vector<std::string>	&splitLine( _parsLine.getSplitLine() );

	if (splitLine.size() != 3)
		throw (MyException("Needs two arguments", MyException::ELVL_WARNING, splitLine.front()));
	else if (!_isHTTPErrorCode(splitLine[1]))
		throw (MyException("Not a valid HTTP error code", MyException::ELVL_WARNING, splitLine.at(1)));
	else if (_errorPages.find(splitLine[1]) != _errorPages.end())
		throw (MyException("Error page is already defined", MyException::ELVL_WARNING, splitLine.at(1)));
	_errorPages.insert(std::pair<std::string, std::string>(splitLine.at(1), splitLine.at(2)));
}


Location::ParsLocation::ParsLocation(ParsLine &parsLine)
:	_parsLine(parsLine),
	_fDefined(0),
	_autoindex(_defaultAutoIndex),
	_allow( GET_MASK(S_GET) | GET_MASK(S_POST) | GET_MASK(S_DELETE) )
{
	_location = _parsLine.getSplitLine().at(1);
	while (_parsLine.readLine(_nTabLocation))
	{
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


void	Location::ParsLocation::printfLocation(void) const
{
	std::cout << "	Location " << _location << ":\n";
	std::cout << "		Root: " << (_isDefined(S_root) ? _root : "UNDEFINED") << "\n";
	std::cout << "		Index:";
	{
		for (std::vector<std::string>::const_iterator it = _index.begin(); it != _index.end(); ++it)
			std::cout << "	" << *it << '\n';
		if (_index.size() == 0)
			std::cout << "	" << "Empty" << '\n';
	}
	std::cout << "		AutoIndex: " << (_autoindex ? CGREEN "ON" : CRED "OFF") << CRESET"\n";
	std::cout << "		AllowedMethods:";
	std::cout << " " << (_isAllowed(S_GET) ? CGREEN : CRED) << "GET" << CRESET;
	std::cout << " " << (_isAllowed(S_POST) ? CGREEN : CRED) << "POST" << CRESET;
	std::cout << " " << (_isAllowed(S_DELETE) ? CGREEN : CRED) << "DELETE" << CRESET << '\n';
	std::cout << "		UploadStore: " << (_isDefined(S_upload_store) ? _uploadLocation : "UNDEFINED") << "\n";
	_return._printInfo();
}
