/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseAutoIndex.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 14:01:01 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/04 15:45:01 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

#include "Client.hpp"
#include "FStat.hpp"
#include "ReadDir.hpp"

void	Response::_addHTMLElement(const std::string &elemName, const std::string &elemContent)
{
	_body += "<" + elemName + ">";
	_body += elemContent;
	_body += "</" + elemName + ">" + endOfLine;
}

void	Response::_AI_addLink(const std::string &fName, const FStat::FILETYPE &fType)
{
	if (fType != FStat::UNKNOWN)
	{
		_body += "<li>";
		_body += "<a href=http://";
		_body += Location::simplifyLocationPath(_cl->getConfig().getIpPortStr()
			+ "/" + _cl->getTargetLocation() + '/' + fName);
		_body += ">";
		_body += fName + (fType == FStat::DIRECTORY ? "/" : "");
		_body += "</a></li>" + endOfLine;
	}
	else
	{
		// _addHTMLElement("a", fName);
		_addHTMLElement("h", "- " + fName);
	}
}

void	Response::generateAutoIndex(ReadDir rDir)
{
	if (rDir.fail())
		return ;// Err
	std::string		nextFileName;
	
	_body += "<!DOCTYPE html>" + endOfLine;
	_body += "<html>" + endOfLine;
	_addHTMLElement("title", _cl->getTargetLocation());
	_body += "<body>" + endOfLine;
	_addHTMLElement("h1", "AutoIndex " + _cl->getTargetLocation());
	do
	{
		nextFileName = rDir.getNextFile();
		if (!nextFileName.empty())
		{
			_AI_addLink(nextFileName, rDir.getFileType());
		}
	}	while (!nextFileName.empty());
	_body += "</body>" + endOfLine;
	_body += "</html>" + endOfLine;
}
