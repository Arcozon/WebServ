/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Return.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 14:25:40 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/05 15:43:20 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Return.hpp"

Return::Return(void)
:	_type(Return::UNDEF)
{}

Return::~Return(void)
{}

Return::Return(const std::string &code, const std::string &url)
:	_type(Return::RET_ERR_URL)
{
	if (!_isHTTPErrorCode(code))
		throw (MyException("is not a valid HTTP error", MyException::ELVL_ERROR, code));
	_argOne = code;
	_argTwo = url;
}

Return::Return(const std::string &text)
:	_type(Return::RET_ERR)
{
	_argOne = text;
	(void)_type;
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

void	Return::_printInfo(void) const
{
	if (_type == RET_ERR)
		std::cout << "Text: " << _argOne << "\n";
	else if (_type == RET_ERR_URL)
		std::cout << "Err: " << _argOne << ", URL: " << _argTwo << "\n";
	else
		std::cout << "Undef" << '\n';
}
