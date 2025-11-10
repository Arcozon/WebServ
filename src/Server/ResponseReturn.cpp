/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseReturn.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 15:57:54 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/10 18:47:14 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Return.hpp"

void	Response::_handleReturn(const Return &ret)
{
	_isReturn = true;
	_responseCode = ret.getCode();
	if (ret.isCode())
		_handleError();
	else if (ret.isBody())
		_body = ret.getArgTwo();
	else if (ret.isRedirect())
	{
		_header["Location"] = ret.getArgTwo();
		_body.clear();
	}
}
