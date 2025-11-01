#include "Response.hpp"

Response::Response(): _response_code(200), _send_count(0), _fully_sent(0)
{
	_body = "<html><body><h1>Hello</h1></body></html>";

}

void Response::setHeaders(std::map<std::string, std::string> &map)
{
	for (std::map<std::string, std::string>::iterator it = map.begin(); it != map.end(); ++it)
		it->second;
}

void Response::setBody(const std::string &body)
{
	//_body += body;
	(void)body;
	_response_buffer += "HTTP/1.1 200 OK\r\n";
	_response_buffer += "Content-Type: text/html; charset=UTF-8\r\n";
	_response_buffer += "Content-Length: 40\r\n";
	_response_buffer += "Connection: close\r\n";
	_response_buffer += "\r\n";
	_response_buffer += _body;
}

void Response::prepare()
{
	_response_buffer += _body;
}

void Response::send(int fd)
{
	while (_send_count < _response_buffer.length())
	{
		size_t to_send = _response_buffer.length() - _send_count;
		int sent = write(fd, _response_buffer.c_str() + _send_count, to_send);
		if (sent > 0)
		{
			_send_count += sent;
			std::cout << "\e[1;32mSent " << sent << " bytes (Remaining: " << _response_buffer.length() -_send_count 
										<< ")\e[0m" << std::endl;
		}
		else
		{
			std::cout << "\e[1;33mSocket buffer full, waiting for next EPOLLOUT " << "\e[0m" << std::endl;
			return ;
		}
	}
	if(_send_count ==_response_buffer.length())
		_fully_sent = 1;
	std::cout << "\e[1;32mResponse sent to client\e[0m" << std::endl;
}

Response::~Response()
{}

bool Response::isResponseFullySent(){return _fully_sent;};