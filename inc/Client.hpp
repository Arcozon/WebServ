#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"

class Client
{
private:
	// struct epoll_event _s_client_event;
	// struct sockaddr_in _s_client_addr;
	// struct epoll_event _s_epoll_event;

	int _fd;
	unsigned int _request_len;
	std::string _str_buffer;
	std::string _extract_line;
	size_t _pos;
	size_t _last_pos;

	typedef enum E_REQUEST_STEP
	{
		REQUEST_LINE,
		HEADERS,
		BODY,
		FIN,
		ERROR
	} REQUEST_STEP;

	REQUEST_STEP _request_step;
	

public:
	Client(/* args */);
	Client(int fd);
	~Client();

	void readFromFd();
	void checkStep();
	int checkCurrentLine();
};


#endif