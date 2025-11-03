#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <string>
# include <map>
# include <sstream>
# include <unistd.h>

class Client;
class IpPort;
class Location;
class Return;

class Response
{
private:
	static const std::string	endOfLine;
	static const std::string	sepNameContent;

private:
	unsigned short	_responseCode;
	std::string		_reasonPhrase;
	std::map<std::string, std::string> _header;
	std::string	_body;

	std::string		_response_buffer;
	unsigned int	_send_count;
	bool			_fully_sent;
	
	Client			*_cl;
	const IpPort	&_ipPort;
	const Location	*_location;

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
	
public:
	Response(Client *cl, const IpPort &ipPort);
	// Response(Client *cl);
	~Response();

	void	setHeaders(const std::map<std::string, std::string> &map);
	void	catResponse(void);

	void	send(int fd);
	void	prepare();
	void	prepare(const std::string &body);
	bool	isResponseFullySent()	const;
};

#endif
