/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 13:05:09 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/29 14:59:04 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <string>
# include <map>
# include <sstream>
#include <vector>
# include <unistd.h>

# include "FStat.hpp"

#define HTTP_VERSION "HTTP/1.1"
#define SERVER_HEADER "TURBOINTGAEUDESSAAL-KUR 5.7.12 (MacOS)"

class Client;
class IpPort;
class Cookies;
class Location;
class Return;
class ReadDir;

class Response
{
private:
	class	MakeHTML;
	static const std::string	endOfLine;
	static const std::string	sepNameContent;
	static std::map<int, std::string> _reason_phrases;

private:
	unsigned short	_responseCode;
	std::string		_reasonPhrase;
	std::map<std::string, std::string> _header;
	std::string _status_line;
	std::string		_body;
	
	bool			_isCGI;
	bool			_isReturn;
	bool			_insideErr;
	
	std::string		_response_buffer;
	unsigned int	_send_count;
	bool			_fully_sent;
	
	std::vector<Cookies> _cookies;
	Client			*_cl;
	const IpPort	&_ipPort;
	const Location	* _location;
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
	void	catCGI(void);
	
	bool	fileToBody(char const fName[]);

	bool	lookForIndex(const char dName[]);	// Returns true if one index was found

	void	_AI_addLink(const std::string &fName, const FStat::FILETYPE &fType, MakeHTML &maker);
	void	generateAutoIndex(ReadDir rDir);

	void	_makeErrPage();

	void	_handleGET(void);
	void	_handlePOST(void);
	void	_handleDELETE(void);

	void	_handleCGI(void);

	void	_handleError(void);
	void	_handleReturn(const Return &ret);

	void	setHeader(const std::string &key, const std::string &val);
	
public:
	static bool	isErrorCode(const unsigned short errCode);
	
	Response(Client *cl, const IpPort &ipPort);
	~Response();

	void	catResponse(void);
	void	makeRep(void);

	void	send(int fd);
	void	prepare();
	bool	isResponseFullySent()	const;

	void setLocation(void);
	void setStartLine(int code);
	void setBody(const std::string &body);

	std::string getReasonPhrase(int code) const;
	void addCookie(const Cookies &cookie);
};

#endif
