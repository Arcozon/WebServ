#include "Client.hpp"

Client::Client(int fd): _fd(fd), _request_len(0),_last_pos(0), _request_step(REQUEST_LINE)
{
}

Client::~Client()
{
	close(_fd);
}
int Client::checkCurrentLine()
{
	_pos = _str_buffer.find("\r\n");
	if(_pos == std::string::npos)
		return 0;
	else
	{
		std::cout << "Got full request line" << std::endl << _str_buffer.substr(_last_pos, _pos + 2) << std::endl;
		_last_pos = _pos + 2;
		return 1;
	}
}

void Client::checkStep()
{
	while(1)
	{
		if(_request_step == REQUEST_LINE)
		{
			if(!checkCurrentLine())
				return ;
			else if((_request_step = HEADERS))
				return ;
		}
		else if(_request_step == HEADERS)
		{
			break ;
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
			std::cout << "Read " << rd << " bytes: " << _str_buffer.substr(_request_len - rd) << std::endl;
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