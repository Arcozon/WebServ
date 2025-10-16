/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsLine.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 12:28:11 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/16 12:40:00 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParsLine.hpp"

ParsLine::ParsLine(const std::string &fileName)
:	_needNewline(true)
{
	_file.open(fileName);
}

ParsLine::~ParsLine(void)
{
	_file.close();
}

bool	ParsLine::_isLineEmpty(void)
{
	return (_line.empty() || _getTabDepth(_line) == std::string::npos);
}

std::size_t	ParsLine::getTabDepth(void) const
{
	return (_tabDepth);
}

bool	ParsLine::readLine(void)
{
	if (_needNewline)
	{
		std::getline(_file, _line);
		if (_file.eofbit)
			return (false);
		_splitLine = _split(_line);
	}
	return (true);
}

bool	ParsLine::fail(void) const
{
	return (_file.fail());
}
