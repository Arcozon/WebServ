/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyException.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 11:44:19 by gaeudes           #+#    #+#             */
/*   Updated: 2025/12/12 17:43:24 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MyException.hpp"

const std::string	MyException::_colorLvl[ELVL_MAX] = {CMAGENTA, CRED, CBRIGHTRED};
const std::string	MyException::_errLvl[ELVL_MAX]   = {"warning: ", "error: ", "fatal error: "};
const MyException::eLevel	MyException::_lvlThrowDown = MyException::ELVL_FATAL;

MyException::MyException(const std::string &err, const eLevel level,
		const std::string &context)
:	_err(err),
	_context(context.empty() ? "" : "[" + context + "]: "),
	_level(level),
	_err_msg(CBOLD + _colorLvl[_level] + _errLvl[_level] + CNONBOLD + _context + _err + CRESET)
{}

MyException::~MyException(void)	throw()
{}

const char	*MyException::what(void) const throw()
{
	return (_err_msg.c_str());
}

const std::string	&MyException::getErrMsg(void) const throw()
{
	return (_err_msg);
}

std::ostream	&operator<<(std::ostream &os, const MyException &exp)
{
	return (os << exp.getErrMsg() + "\n");
}
