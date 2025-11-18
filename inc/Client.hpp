#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Server.hpp"
# include "Response.hpp"

# include <string>
# include <map>
# include <vector>
# include <algorithm>

#define MAX_HEADER_SIZE 8192
#define HEADERS_TOTAL_MAX 8192

class IpPort;
class Response;
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

	static const std::string	_sepLine;
	static const std::size_t	_sepLineLen;

	static const std::string	_supportedHTTPVersion;
	
	static const std::size_t	_bufferSize;
	
private:
	// struct epoll_event _s_client_event;
	// struct sockaddr_in _s_client_addr;
	// struct epoll_event _s_epoll_event;

	int _fd;
	IpPort *_config;
	unsigned int _request_len;
	std::string _str_buffer;
	std::string _extract_line;
	size_t _pos;
	size_t _last_pos;
	std::map<std::string, std::string> _headers;
	
	REQUEST_STEP	_request_step;
	bool			_done;
	Response *_response;

	bool	_is_upload;
	std::string _method;
	std::string _target_uri;
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
	Client(/* args */);
	Client(int fd, IpPort *config, Sessions *instance);
	~Client();

	void readFromFd();
	void checkStep();
	bool checkCurrentLine();
	void sendResponse();
	bool responseSent();
	bool finishedReading();
	void fileHandler();
	void updateTimer();
	bool checkTimers();
	bool timedOut();
	std::string getCookie(const std::string &name) const;
	const std::map<std::string, std::string> &getCookies() const;

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
