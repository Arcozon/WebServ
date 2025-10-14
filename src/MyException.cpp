/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyException.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 11:44:19 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/14 13:28:45 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MyException.hpp"

const std::string	MyException::_colorLvl[ELVL_MAX] = {CMAGENTA, CRED, CBOLD CRED};
const std::string	MyException::_errLvl[ELVL_MAX]   = {"warning: ", "error: ", "fatal error: "};

#include <iostream>

MyException::MyException(const std::string &err, const enum eLevel level,
		const std::string &context)
:	_err(err), _context(context.empty() ? "" : context + ": "), _level(level),
	_err_msg(_colorLvl[_level] + _context + _errLvl[_level] + _err + CRESET)
{}

MyException::~MyException(void)	throw()
{}

const char	*MyException::what(void) const throw()
{
	return (_err.c_str());
}

const std::string	&MyException::getErrMsg(void) const throw()
{
	return (_err_msg);
}


std::ostream	&operator<<(std::ostream &os, const MyException &exp)
{
	return (os << exp.getErrMsg() + "\n");
}
