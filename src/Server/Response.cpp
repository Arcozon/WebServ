#include "Response.hpp"
#include "Client.hpp"
#include "Cookies.hpp"

std::map<int, std::string> Response::_reason_phrases;

Response::Response(Client *cl): _response_code(200), _send_count(0), _fully_sent(0), _cl(cl)
{
	_body = "";
	if(_reason_phrases.empty())
	{
		_reason_phrases[200] = "OK";
		_reason_phrases[201] = "Created";
		_reason_phrases[204] = "No Content";

		_reason_phrases[400] = "Bad Request";
		_reason_phrases[403] = "Forbidden";
		_reason_phrases[404] = "Not Found";
		_reason_phrases[405] = "Method Not Allowed";
		_reason_phrases[411] = "Length Required";
		_reason_phrases[413] = "Payload Too Large";
		_reason_phrases[414] = "URI Too Long";
		_reason_phrases[431] = "Request Header Fields Too Large";

		_reason_phrases[500] = "Internal Server Error";
		_reason_phrases[501] = "Not Implemented";
		_reason_phrases[503] = "Service Unavailable";
		_reason_phrases[505] = "HTTP Version Not Supported";
		
	}
	setHeader("Server", SERVER_HEADER);
	setHeader("Connection", "close");
	setHeader("Content-Type", "text/html; charset=UTF-8");

}

void Response::setStartLine(int code)
{
	_response_code = code;
	std::ostringstream res;
	res << HTTP_VERSION << " " << code << " " << getReasonPhrase(code) << "\r\n";
	_status_line = res.str();
}

void Response::setHeader(const std::string &key, const std::string &val)
{
	_headers[key] = val;
}

void Response::setBody(const std::string &body)
{
	//_body += body;
	// _response_buffer += "HTTP/1.1 200 OK\r\n";
	// _response_buffer += "Content-Type: text/html; charset=UTF-8\r\n";
	// _response_buffer += "Content-Length: 40\r\n";
	// _response_buffer += "Connection: close\r\n";
	// _response_buffer += "\r\n";
	if(!body.empty())
	{	
		_body = body;
		std::ostringstream len;
		len << _body.length();
		setHeader("Content-Length", len.str());
	}
	
}

void Response::prepare()
{
	std::ostringstream len, headers;
	if (_headers.find("Content-Length") == _headers.end()  && !_body.empty())
	{
		len << _body.length();
		setHeader("Content-Length", len.str());
	}

	for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it)
		headers << it->first << ": " << it->second << "\r\n";

	for (size_t i = 0; i < _cookies.size(); i++)
		headers << "Set-Cookie: " << _cookies[i].setCookieHeader() << "\r\n";

	headers << "\r\n";
	setStartLine(_response_code);
	_response_buffer += _status_line;
	_response_buffer += headers.str();
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
			_cl->updateTimer();
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

std::string Response::getReasonPhrase(int code) const
{
	std::map<int, std::string>::iterator it = _reason_phrases.find(code);
	if (it != _reason_phrases.end())
		return it->second;
	else
		return "GAEUDES";
}

void Response::addCookie(const Cookies &cookie)
{
	_cookies.push_back(cookie);
}
