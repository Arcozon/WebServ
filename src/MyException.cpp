/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyException.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 11:44:19 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/14 12:39:33 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MyException.hpp"

const std::string	MyException::_colorLvl[ELVL_MAX] = {CMAGENTA, CBOLD CRED, CRED};
const std::string	MyException::_errLvl[ELVL_MAX] = {"warning: ", "error: ", "fatal error: "};


MyException::MyException(const std::string &err, const std::string &context = "",
		const enum eLevel level)
:	_err(err), _context(context.empty() ? "" : context + ": "), _level(level)
{}

MyException::~MyException(void)	throw()
{}

const char	*MyException::what(void) const throw()
{
	return (_err.c_str());
}

const std::string	&MyException::getErrMsg(void) const throw()
{
	return (_colorLvl[_level] + _context + _errLvl[_level] + _err + CRESET);
}


std::ostream	&operator<<(std::ostream &os, const MyException &exp)
{
	return (os << exp.getErrMsg());
}
