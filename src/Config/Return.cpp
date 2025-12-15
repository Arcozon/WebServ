/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Return.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 14:25:40 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/10 18:38:07 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Return.hpp"

Return::Return(void)
:	_type(Return::UNDEF)
{}

Return::~Return(void)
{}

Return::Return(const std::string &arg1, const std::string &arg2)
:	_type(Return::UNDEF)
{
	if (!_isHTTPErrorCode(arg1))
		throw (MyException("is not a valid HTTP error", MyException::ELVL_ERROR, arg1));
	_argOne = arg1;
	_argTwo = arg2;
	if (_argOne[0] == '3')
		_type = Return::CODE_URL;
	else
		_type = Return::CODE_BODY;
}

Return::Return(const std::string &code)
:	_type(Return::UNDEF)
{
	if (!_isHTTPErrorCode(code))
		throw (MyException("is not a valid HTTP error", MyException::ELVL_ERROR, code));
	_type = Return::CODE;
	_argOne = code;
}

Return	&Return::operator=(const Return &other)
{
	if (this == &other)
		return (*this);
	_type = other._type;
	_argOne = other._argOne;
	_argTwo = other._argTwo;
	return (*this);
}

const std::string	&Return::getCodeStr(void) const
{
	return (_argOne);
}

int	Return::getCode(void) const
{
	return (std::atoi(_argOne.c_str()));
}

const std::string	&Return::getArgTwo(void) const
{
	return (_argTwo);
}

bool	Return::isDefined(void) const
{
	return (_type != UNDEF);
}

bool	Return::isCode(void) const
{
	return (_type == CODE);
}

bool	Return::isRedirect(void) const
{	
	return (_type == CODE_URL);
}

bool	Return::isBody(void) const
{
	return (_type == CODE_BODY);
}

void	Return::_fixUrlWithDomainName(const std::string &domain)
{
	if (_type != CODE_URL)
		return ;
	if (_argTwo.find("http://") == 0 || _argTwo.find("https://") == 0)
		return ;
	if (_argTwo.at(0) != '/')
		_argTwo.insert(0, 1, '/');
	std::string	newArgTwo = "http://" + domain;
	if (domain.at(domain.size() - 1) == '/')
		_argTwo.erase(_argTwo.size() - 1);
	newArgTwo += _argTwo;
	_argTwo = newArgTwo;
}

void	Return::_printInfo(void) const
{
	if (_type == CODE)
		std::cout << "Code: " << _argOne << "\n";
	else if (_type == CODE_URL)
		std::cout << "Code: " << _argOne << ", URL: " << _argTwo << "\n";
	else if (_type == CODE_BODY)
		std::cout << "Code: " << _argOne << ", Body: '" << _argTwo << "'\n";
	else
		std::cout << "Undef" << '\n';
}

bool	Return::_isHTTPErrorCode(const int code)
{
	return (code >= 100 && code < 600);
}

bool	Return::_isHTTPErrorCode(const std::string &code)
{
	return ( (code.size() == 3)
		&& (code.find_first_not_of("0123456798") == std::string::npos)
		&& (code[0] >= '1' && code[0] <= '5') );
}
