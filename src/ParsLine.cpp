/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsLine.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 12:28:11 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/16 15:16:10 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParsLine.hpp"

ParsLine::ParsLine(void)
:	_needNewline(true),
	_tabDepth(0)
{}

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

void	ParsLine::open(const std::string &fileName, std::ios_base::openmode mode)
{
	_file.close();
	_file.open(fileName.c_str(), mode);
}

bool	ParsLine::_isLineEmpty(void)
{
	return (_line.empty() || _getTabDepth(_line) == std::string::npos);
}

bool	ParsLine::readLine(const std::size_t expectedTab)
{
	if (_file.fail())
	{
		return (false);
	}
	else if (_needNewline)
	{
		do
		{
			if (!std::getline(_file, _line))
				return (false);
		}	while (_isLineEmpty());
		_tabDepth = getTabDepth(_line);
		_splitLine = _split(_line);
	}
	_needNewline = ( _tabDepth >= expectedTab );
	return (_needNewline);
}
