#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <string>
#include <map>
#include <unistd.h>

class Client;

class Response
{
private:
	std::map<std::string, std::string> _headers;
	std::string _body;
	std::string _response_buffer;
	short int _response_code;
	unsigned int _send_count;
	bool _fully_sent;
	//Client *_cl;

	public:
	Response();
	~Response();
	void setHeaders(std::map<std::string, std::string> &map);
	void send(int fd);
	void prepare();
	void setBody(const std::string &body);
	bool isResponseFullySent();
};




#endif