/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Return.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 14:25:40 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/15 14:49:29 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Return.hpp"

Return::Return(void)
:	_type(Return::RET_TEXT)
{}

Return::~Return(void)
{}

// Valid HTTP ErrCode are in range [100, 600[
static inline bool	_isHTTPErrorCode(const std::string &str)
{
	return ( (str.size() == 3)
		&& (str.find_first_not_of("0123456798") == std::string::npos)
		&& (str[0] >= '1' && str[0] <= '5') );
}

Return::Return(const std::string &code, const std::string &url)
:	_type(Return::RET_ERR_URL)
{
	if (!_isHTTPErrorCode(code))
		throw (MyException("is not a valid HTTP error", MyException::ELVL_ERROR, code));
	// Check URL
	_argOne = code;
	_argTwo = url;
}

Return::Return(const std::string &text)
:	_type(Return::RET_TEXT)
{
	_argOne = text;
	(void)_type;
}
