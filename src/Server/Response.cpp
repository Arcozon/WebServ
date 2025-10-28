#include "Response.hpp"

Response::Response(): _response_code(200)
{
}

void Response::setHeaders(std::map<std::string, std::string> &map)
{
	for (std::map<std::string, std::string>::iterator it = map.begin(); it != map.end(); ++it)
		it->second;
}

