#include "Client.hpp"


const std::string	Client::_sepLine = "\r\n";
const std::size_t	Client::_sepLineLen = _sepLine.size();

Client::Client(int fd): _fd(fd), _request_len(0),_last_pos(0), _request_step(REQUEST_LINE)
{
}

Client::~Client()
{
	close(_fd);
}

bool	Client::_makeExtractLine(void)
{
	_pos = _str_buffer.find(_sepLine, _last_pos);
	if(_pos == std::string::npos)
		return (false);
	else
	{
		//std::cout << "Got full request line" << std::endl << _str_buffer.substr(_last_pos, _pos + 2) << std::endl;
		_extract_line = _str_buffer.substr(_last_pos, _pos - _last_pos);
		// std::cout << "Full line recieved: " << _extract_line << std::endl;
		std::cout  << "\e[34m"<< _extract_line << "\e[0m" << std::endl;
		_last_pos = _pos + _sepLineLen;
		return (true);
	}
}

bool Client::checkCurrentLine()
{
	if (_makeExtractLine())
		return (false);
	return (true);
}

bool	Client::_checkRequestLine(void)	// Add IoPort (to check )
{

}

bool	Client::_checkHeader(void)	// Add IoPort (to check )
{

}

bool	Client::_checkBody(void)	// Add IoPort (to check )
{

}

void Client::checkStep()
{
	while (_request_step != FIN && _request_step != ERROR)
	{
		if(_request_step == REQUEST_LINE)
		{
			if(!checkCurrentLine())
				return ;
			else
				_request_step = HEADERS;
		}
		else if(_request_step == HEADERS)
		{
			if(!checkCurrentLine())
				return ;
			if(_extract_line.empty())
			{
				std::cout << "End of headers found" << std::endl;
				_request_step = BODY;
			}
		}
		else if(_request_step == BODY)
		{
			if(!checkCurrentLine())
				return ;
			else
			{
				_request_step = FIN;
				break;
			}
		}
	}

}

void Client::readFromFd()
{
	char buffer[10];
	while(1)
	{
		int rd = read(_fd, buffer, 10);
		if(rd > 0)
		{
			_str_buffer.append(buffer, rd);
			_request_len += rd;
			checkStep();
			if(_request_len == ERROR)
				;
			//std::cout << "Read " << rd << " bytes: " << _str_buffer.substr(_request_len - rd) << std::endl;
		}
		else if(rd == -1)
			return ;
		else if(rd == 0)
		{
			std::cout << "Client closed connection on fd: " << _fd << std::endl;
			break ;
		}
	}
	std::cout << "Client on fd [" << _fd << "] recieved a total of " << _request_len << " bytes:" << std::endl
	<< _str_buffer << std::endl;
}
