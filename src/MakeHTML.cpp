/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MakeHTML.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 11:17:41 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/10 11:39:44 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "makeHTML.hpp"

Response::makeHTML::makeHTML(std::string &body)
:	_body(body)
{
	_body = "<!DOCTYPE html>";
}

Response::makeHTML::~makeHTML(void)
{
	while (closeMarkup())
		;
}


void	Response::makeHTML::addMarkup(const std::string &markupName, const std::string &bonusContext, const bool &toClose)
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

bool	Response::makeHTML::closeMarkup(void)
{
	if (_markups.empty())
		return (false);
		
	_body += "</" + _markups.top() + ">";
	_markups.pop();
	return (true);
}


void	Response::makeHTML::addText(const std::string &text)
{
	_body += text;
}

void	Response::makeHTML::addMarkupWithText(const std::string &markupName,
							const std::string &bonusContext,
							const std::string &text)
{
	addMarkup(markupName, bonusContext);
	addText(text);
	closeMarkup();
}

void	Response::makeHTML::addMarkupWithText(const std::string &markupName,
							const std::string &text)
{
	addMarkup(markupName);
	addText(text);
	closeMarkup();
}

