#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Server.hpp"

# include <string>
# include <map>
# include <vector>
# include <algorithm>

class IpPort;
class Response;
class Location;

class Client
{
private:

	typedef enum E_REQUEST_STEP
		{
			REQUEST_LINE,
			HEADERS,
			BODY,
			FIN,
			ERROR
		} REQUEST_STEP;

	static const std::size_t	_bufferSize;
	static const std::string	_sepLine;
	static const std::size_t	_sepLineLen;

	static const std::string	_supportedHTTPVersion;
	
	
private:
	// struct epoll_event _s_client_event;
	// struct sockaddr_in _s_client_addr;
	// struct epoll_event _s_epoll_event;

	int				_fd;
	const IpPort	&_config;
	
	std::size_t	_request_len;
	std::string	_strBuffer;
	size_t		_pos;
	std::string	_extractedLine;
	REQUEST_STEP	_requestStep;
	
	std::string	_method;
	std::string	_requestTarget;
	std::string	_HTTPVersion;
	std::map<std::string, std::string> _headers;
	
	Location *_location;
	Response *_response;
	
private:
	static const std::vector<std::string>	_splitRequestLine(const std::string &reqLine);
	static const std::pair<std::string, std::string>	_splitHeaderLine(const std::string &reqLine);

	bool	_makeExtractLine(void);
	bool	_checkRequestLine(void);	// Add IoPort (to check Allowed Methods and other)
	bool	_checkHeader(void);	// Add IoPort (to check )
	bool	_checkBody(void);	// Add IoPort (to check )
	bool	_checkCurrentLine(const Client::REQUEST_STEP &reqSection);
	
public:
	// Client(int fd, IpPort *config);
	Client(int fd, const IpPort &config);
	~Client();

	void readFromFd();
	void checkStep();
	void sendResponse();
	bool responseSent() const;
	bool finishedReading() const;

	const IpPort		&getConfig(void)	const;
	const std::string	&getMethod(void)	const;
	const std::string	&getTargetLocation(void)	const;
};


#endif

// GET /home.html HTTP/1.1
// Host: developer.mozilla.org
// User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.9; rv:50.0) Gecko/20100101 Firefox/50.0
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
// Accept-Language: en-US,en;q=0.5
// Accept-Encoding: gzip, deflate, br
// Referer: https://developer.mozilla.org/testpage.html
// Connection: keep-alive
// Upgrade-Insecure-Requests: 1
// If-Modified-Since: Mon, 18 Jul 2016 02:36:04 GMT
// If-None-Match: "c561c68d0ba92bbeb8b0fff2a9199f722e3a621a"
// Cache-Control: max-age=0
