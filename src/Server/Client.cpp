#include "Client.hpp"

const std::string	Client::_supportedHTTPVersion = "HTTP/1.1";
const std::string	Client::_sepLine = "\r\n";
const std::size_t	Client::_sepLineLen = _sepLine.size();

Client::Client(int fd)
:	_fd(fd),
	_request_len(0),
	_last_pos(0),
	_request_step(REQUEST_LINE),
	_done(false)
{
}

Client::~Client()
{
	close(_fd);
}

bool	Client::_makeExtractLine(void)
{
	// _pos = _str_buffer.find(_sepLine, _last_pos);
	// if(_pos == std::string::npos)
	// 	return (false);
	// else
	// {
	// 	//std::cout << "Got full request line" << std::endl << _str_buffer.substr(_last_pos, _pos + 2) << std::endl;
	// 	_extract_line = _str_buffer.substr(_last_pos, _pos - _last_pos);
	// 	// std::cout << "Full line recieved: " << _extract_line << std::endl;
	// 	// std::cout  << "\e[34m"<< _extract_line << "\e[0m" << std::endl;
	// 	_last_pos = _pos + _sepLineLen;
	// 	return (true);
	// }
	static const std::size_t	bufferSize = 2;
	char						buffer[bufferSize + 1];
	int							rd;

	do
	{
		_pos = _str_buffer.find(_sepLine, _last_pos);
		if (_pos != std::string::npos)
		{
			_extract_line = _str_buffer.substr(_last_pos, _pos - _last_pos);
			_last_pos = _pos + _sepLineLen;
			std::cout << "line: " << _extract_line << '\n';
			return (true);
		}
		rd = read(_fd, buffer, bufferSize);
		if(rd > 0)
		{
			_str_buffer.append(buffer, rd);
			_request_len += rd;
		}
		else if(rd < 0)
			return (false);
	}	while (rd != 0);
	_done = true;
	return (true);
}

const std::vector<std::string>	Client::_splitRequestLine(const std::string &reqLine)
{
	static const char	setReqLine = ' ';	
	std::vector<std::string>	splitReqLine;

	if (std::count(reqLine.begin(), reqLine.end(), setReqLine) != 2)
		return (splitReqLine);

	std::size_t	firstSpace = reqLine.find_first_of(setReqLine);
	std::size_t	secondSpace = reqLine.find_first_of(setReqLine, firstSpace + 1);

	splitReqLine.push_back(reqLine.substr(0, firstSpace));
	splitReqLine.push_back(reqLine.substr(firstSpace + 1, secondSpace - (firstSpace + 1)));
	splitReqLine.push_back(reqLine.substr(secondSpace + 1));
	return (splitReqLine);
}

const std::pair<std::string, std::string>	Client::_splitHeaderLine(const std::string &reqLine)
{
	static const std::string	nameValSep = ": ";

	std::pair<std::string, std::string>	splitHeaderLine;

	std::size_t	indexSep = reqLine.find(nameValSep);
	// std::cout << "Index: " << indexSep << std::endl; 
	if (indexSep == std::string::npos)
		return (splitHeaderLine);
	splitHeaderLine.first = reqLine.substr(0, indexSep); 
	splitHeaderLine.second = reqLine.substr(indexSep + nameValSep.size()); 
	return (splitHeaderLine);
}

bool	Client::_checkRequestLine(void)	// Add IpPort (to check )
{
	std::vector<std::string>	splitReqLine = _splitRequestLine(_extract_line);

	if (splitReqLine.empty())
		return (false);
	
	std::string	method = splitReqLine.at(0);
	std::string	requestTarget = splitReqLine.at(1);
	std::string	HTTPVersion = splitReqLine.at(2);

	if (!(method == "GET" || method == "POST" || method == "DELETE"))
		return (false);
	// Check if method is in IpPort / location
	// check if request target is in IoPort / if [Root]/[RequestTarget] is a dir
	if (HTTPVersion != _supportedHTTPVersion)
		return (false);
	std::cout << "\e[32m[" << method << "]\e[33m[" << requestTarget << "]\e[34m[" << HTTPVersion << "]\e[0m" << std::endl;
	return (true);
	(void) requestTarget;
}

bool	Client::_checkHeader(void)	// Add IpPort (to check )
{
	if (_extract_line.empty())
	{
		std::cout  << "\e[1:31m"<< "	-- End Of Header --" << "\e[0m" << std::endl;
		_request_step = BODY;
		return (true);
	}

	std::pair<std::string, std::string>	_nameVal = _splitHeaderLine(_extract_line);

	if (_nameVal.first.empty())
		return (false);
	_headers.insert(_nameVal);
	std::cout  << "\e[35m["<< _nameVal.first << "]\e[34m[" << _nameVal.second  << "]\e[0m" << std::endl;
	return (true);
}

bool	Client::_checkBody(void)	// Add IpPort (to check )
{
	std::cout  << "\e[1;37m"<< _extract_line << "\e[0m" << std::endl;
	return (true);
}

bool Client::_checkCurrentLine(const Client::REQUEST_STEP &reqSection)
{
	if (!_makeExtractLine())
		return (false);

	switch (reqSection)
	{
		case (REQUEST_LINE):
			return (_checkRequestLine());
		case (HEADERS):
			return (_checkHeader());
		case (BODY):
			return (_checkBody());
		default:
			return (false);
	}
}

void Client::checkStep()
{
	if (!_checkCurrentLine(REQUEST_LINE))
	{
		_request_step = ERROR;
		return ;
	}
	std::cout  << "\e[1:31m"<< "	-- Start Of Header --" << "\e[0m" << std::endl;
	_request_step = HEADERS;
	while (_request_step == HEADERS)
	{
		if (!_checkCurrentLine(HEADERS))
		{
			_request_step = ERROR;
			return ;
		}
	}
	while (_request_step == BODY)
	{
		if (!_checkCurrentLine(REQUEST_LINE))
		{
			_request_step = ERROR;
			return ;
		}
	}
	std::cout  << "\e[1:31m"<< "	-- End Of Body --" << "\e[0m" << std::endl;
	_request_step = FIN;
}

void Client::readFromFd()
{
	while(!_done && (_request_step != ERROR))
	{
		checkStep();
	}
}

// void Client::checkStep()
// {
// 	while (_request_step != FIN && _request_step != ERROR)
// 	{
// 		if(_request_step == REQUEST_LINE)
// 		{
// 			if(!checkCurrentLine())
// 				return ;
// 			else
// 				_request_step = HEADERS;
// 		}
// 		else if(_request_step == HEADERS)
// 		{
// 			if(!checkCurrentLine())
// 				return ;
// 			if(_extract_line.empty())
// 			{
// 				std::cout << "End of headers found" << std::endl;
// 				_request_step = BODY;
// 			}
// 		}
// 		else if(_request_step == BODY)
// 		{
// 			if(!checkCurrentLine())
// 				return ;
// 			else
// 			{
// 				_request_step = FIN;
// 				break;
// 			}
// 		}
// 	}
// }

// void Client::readFromFd()
// {
// 	char buffer[10];
// 	while(1)
// 	{
// 		int rd = read(_fd, buffer, 10);
// 		if(rd > 0)
// 		{
// 			_str_buffer.append(buffer, rd);
// 			_request_len += rd;
// 			checkStep();
// 			if(_request_len == ERROR)
// 				;
// 			//std::cout << "Read " << rd << " bytes: " << _str_buffer.substr(_request_len - rd) << std::endl;
// 		}
// 		else if(rd == -1)
// 			return ;
// 		else if(rd == 0)
// 		{
// 			std::cout << "Client closed connection on fd: " << _fd << std::endl;
// 			break ;
// 		}
// 	}
// 	std::cout << "Client on fd [" << _fd << "] recieved a total of " << _request_len << " bytes:" << std::endl
// 	<< _str_buffer << std::endl;
// }
