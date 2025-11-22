/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MakeHTML.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 11:17:41 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/22 19:00:34 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MakeHTML.hpp"

Response::MakeHTML::MakeHTML(std::string &body)
:	_body(body)
{
	_body = "<!DOCTYPE html>";
}

Response::MakeHTML::~MakeHTML(void)
{
	while (closeMarkup())
		;
}


void	Response::MakeHTML::addMarkup(const std::string &markupName, const std::string &bonusContext, const bool &toClose)
{
	_body += "<" + markupName;
	if (!bonusContext.empty())
	{
		_body += " " + bonusContext;
	}
	_body += ">";
	if (toClose)
		_markups.push(markupName);
}

bool	Response::MakeHTML::closeMarkup(void)
{
	if (_markups.empty())
		return (false);
		
	_body += "</" + _markups.top() + ">";
	_markups.pop();
	return (true);
}


void	Response::MakeHTML::addText(const std::string &text)
{
	_body += text;
}

void	Response::MakeHTML::addMarkupWithText(const std::string &markupName,
							const std::string &bonusContext,
							const std::string &text)
{
	addMarkup(markupName, bonusContext);
	addText(text);
	closeMarkup();
}

void	Response::MakeHTML::addMarkupWithText(const std::string &markupName,
							const std::string &text)
{
	addMarkup(markupName);
	addText(text);
	closeMarkup();
}

