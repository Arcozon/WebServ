#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <string>
#include <map>
#include <unistd.h>
#include <sstream>

#define HTTP_VERSION "HTTP/1.0"
#define SERVER_HEADER "TURBOINTGAEUDES 1.0 (Unix/Summoner's rift/top lane)"

class Client;
class IpPort;

class Response
{
private:
	std::map<std::string, std::string> _headers;
	static std::map<int, std::string> _reason_phrases;
	std::string _status_line;
	std::string _body;
	std::string _response_buffer;
	
	short int _response_code;
	unsigned int _send_count;
	bool _fully_sent;
	Client *_cl;

	public:
	Response(Client *cl);
	~Response();
	void setHeader(const std::string &key, const std::string &val);
	void send(int fd);
	void prepare();
	void setBody(const std::string &body);
	bool isResponseFullySent();
	void setStartLine(int code);
	void setReasonPhrases();
	std::string getReasonPhrase(int code) const;

};




#endif