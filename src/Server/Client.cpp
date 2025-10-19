#include "Client.hpp"

Client::Client(int fd): _fd(fd), _request_len(0)
{
}

Client::~Client()
{
	close(_fd);
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