#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Server.hpp"

# include <string>
# include <map>
# include <vector>
# include <algorithm>

#define MAX_HEADER_SIZE 8192
#define HEADERS_TOTAL_MAX 8192

class IpPort;
class Response;
class Location;
class Cookies;
class Sessions;

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

	int				_fd;
	const IpPort	&_config;
	
	std::size_t	_request_len;
	std::string	_strBuffer;
	size_t		_pos;
	std::string	_extractedLine;
	REQUEST_STEP	_requestStep;
	std::size_t		_last_pos;
	bool			_done;
	
	std::string	_method;
	std::string	_requestTarget;
	std::string	_queryString;
	std::string	_HTTPVersion;
	std::map<std::string, std::string> _headers;
	
	Location *_location;
	Response *_response;

	bool	_is_upload;
	size_t _content_length;
	size_t _body_rd_bytes;
	std::string _body_data;
	std::string _upload_dir;

	time_t _last_activity;
	time_t _read_timer;
    time_t _write_timer;
	time_t _client_spawn;
	time_t _max_req_duration;

	std::map<std::string, std::string> _cookies;
	Sessions *_session_ptr;

	size_t _headers_total_size;

private:
	static const std::vector<std::string>	_splitRequestLine(const std::string &reqLine);
	static const std::pair<std::string, std::string>	_splitHeaderLine(const std::string &reqLine);

	bool	_makeExtractLine(void);
	bool	_checkRequestLine(void);	// Add IoPort (to check Allowed Methods and other)
	bool	_checkHeader(void);	// Add IoPort (to check )
	bool	_checkBody(void);	// Add IoPort (to check )
	bool	_checkCurrentLine(const Client::REQUEST_STEP &reqSection);
	

	bool validVerbSyntax(const std::string &method);
	bool ValidURI(const std::string &uri);
	bool validHeaderSyntax(const std::string &name);
	bool validMinimalHeaders();
	bool validContentLength(const std::string &value);
	bool validDuplicateHeader(const std::string &name);
	bool validHeader(const std::string &name, const std::string &value);



public:
	Client(int fd, const IpPort &config, Sessions *instance);
	~Client();

	void readFromFd();
	void checkStep();
	void sendResponse();
	bool responseSent() const;
	bool finishedReading() const;
	void fileHandler();
	void updateTimer();
	bool checkTimers();
	bool timedOut();
	std::string getCookie(const std::string &name) const;
	const std::map<std::string, std::string> &getCookies() const;
	void putHandler();
	void deleteHandler();

	const IpPort	&getConfig(void) const;
	const std::string	&getMethod(void) const;
	const std::string	&getTargetLocation(void) const;
	const std::string	&getQueryString(void) const;
	const std::map<std::string, std::string>	&getHeader(void) const;
	const std::string	&getBody(void) const;

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
