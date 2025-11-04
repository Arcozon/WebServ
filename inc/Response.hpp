/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 13:05:09 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/04 14:25:51 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <string>
# include <map>
# include <sstream>

# include <unistd.h>

# include "FStat.hpp"

class Client;
class IpPort;
class Location;
class Return;
class ReadDir;

class Response
{
private:
	static const std::string	endOfLine;
	static const std::string	sepNameContent;

private:
	unsigned short	_responseCode;
	std::string		_reasonPhrase;
	std::map<std::string, std::string> _header;
	std::string		_body;

	std::string		_response_buffer;
	unsigned int	_send_count;
	bool			_fully_sent;
	
	Client			*_cl;
	const IpPort	&_ipPort;
	const Location	*_location;
	std::string		_URI;

private:
	inline void	catLine(const std::string &line)
		{	_response_buffer += line + endOfLine;	}

	inline void	catStatusLine(const std::string &responseCode,
		const std::string &reasonPhrase, const std::string &protocol = "HTTP/1.1")
		{	this->catLine(protocol + ' ' + responseCode + ' ' + reasonPhrase);	}

	inline void	catStatusLine(const unsigned int &responseCode,
		const std::string &reasonPhrase, const std::string &protocol = "HTTP/1.1")
		{
			std::ostringstream oss;
			oss << responseCode;
			this->catStatusLine(oss.str(), reasonPhrase, protocol);
		}

	inline void	catHeaderLine(const std::string vName, const std::string vContent)
		{	this->catLine(vName + sepNameContent + vContent);	}
	
	void	catHeader(void);
	void	catContentLenght(void);
	void	catBody(void);
	
	void	fileToBody(char const fName[]);

	bool	lookForIndex(const char dName[]);	// Returns true if one index was found

	void	generateAutoIndex(ReadDir rDir);

	void	_addHTMLElement(const std::string &elemName, const std::string &elemContent);
	void	_AI_addLink(const std::string &fName, const FStat::FILETYPE &fType);
public:
	Response(Client *cl, const IpPort &ipPort);
	// Response(Client *cl);
	~Response();

	void	setHeaders(const std::map<std::string, std::string> &map);
	void	catResponse(void);
	void	mkRepFromLoc(void);

	void	send(int fd);
	void	prepare();
	void	prepare(const std::string &body);
	bool	isResponseFullySent()	const;
};

#endif
