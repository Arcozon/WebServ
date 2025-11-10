/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseAutoIndex.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 14:01:01 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/10 11:52:00 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "MakeHTML.hpp"

#include "Client.hpp"
#include "FStat.hpp"
#include "ReadDir.hpp"

void	Response::_AI_addLink(const std::string &fName, const FStat::FILETYPE &fType, MakeHTML &maker)
{
	if (fType != FStat::UNKNOWN)
	{
		maker.addMarkup("li");
		std::string	link = "href=http://" + Location::simplifyLocationPath(_cl->getConfig().getIpPortStr()
							+ "/" + _cl->getTargetLocation() + '/' + fName);
		maker.addMarkup("a", link);
		maker.addText(fName + (fType == FStat::DIRECTORY ? "/" : ""));
		maker.closeMarkup();
		maker.closeMarkup();
	}
	else
	{
		maker.addMarkupWithText("h", "- " + fName);
	}
}

void	Response::generateAutoIndex(ReadDir rDir)
{
	if (!rDir.fail())
	{
		MakeHTML		maker(_body);
		std::string		nextFileName;
		
		_body += "<!DOCTYPE html>" + endOfLine;
		maker.addMarkup("html");
		maker.addMarkupWithText("title", _cl->getTargetLocation());
		maker.addMarkup("body");
		maker.addMarkupWithText("h1", "AutoIndex " + _cl->getTargetLocation());
		do
		{
			nextFileName = rDir.getNextFile();
			if (!nextFileName.empty())
				_AI_addLink(nextFileName, rDir.getFileType(), maker);
		}	while (!nextFileName.empty());
	}
	else
		_responseCode = 403;
}
