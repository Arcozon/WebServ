#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Client.hpp"

class Response
{
private:
	std::map<std::string, std::string> _headers;
	std::string _body;
	short int _response_code;
	
	public:
	Response(/* args */);
	~Response();
	void prepareResponse();
	int sendResponse();
	void setHeaders(std::map<std::string, std::string> &map);
};




#endif