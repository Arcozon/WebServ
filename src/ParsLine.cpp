/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsLine.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 12:28:11 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/16 14:28:01 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParsLine.hpp"

ParsLine::ParsLine(const std::string &fileName)
:	_needNewline(true),
	_tabDepth(0)
{
	_file.open(fileName.c_str());
}

ParsLine::~ParsLine(void)
{
	_file.close();
}

bool	ParsLine::_isLineEmpty(void)
{
	return (_line.empty() || _getTabDepth(_line) == std::string::npos);
}

bool	ParsLine::readLine(void)
{
	if (_file.fail())
		return (false);
	else if (_needNewline)
	{
		do
		{
			if (!std::getline(_file, _line))
				return (false);
		}	while (_isLineEmpty());
		if (_tabDepth > getTabDepth(_line))
			_needNewline = false;
		_tabDepth = getTabDepth(_line);
		_splitLine = _split(_line);
	}
	else
		_needNewline = true;
	return (true);
}
