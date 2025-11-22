#include "Cookies.hpp"

Cookies::Cookies(const std::string &name, const std::string &value): _name(name), _value(value), _path("/"), _duration(0)
{
}

Cookies::~Cookies(){}

void Cookies::setPath(const std::string &path)
{
	_path = path;
}

void Cookies::setDuration(int seconds)
{
	_duration = seconds;
}

std::string Cookies::setCookieHeader() const
{
	std::ostringstream res;
	res << _name << "=" << _value << "; Path=" << _path;
	
	if (_duration > 0)
		res << "; Max-Age=" << _duration;
	return res.str();
}

std::map<std::string, std::string> Cookies::parseCookie(const std::string &cookie_header)
{
	std::map<std::string, std::string> cookies;
	size_t pos = 0;
	
	while (pos < cookie_header.length())
	{
		while (pos < cookie_header.length() && std::isspace(cookie_header[pos]))
			pos++;

		size_t end = cookie_header.find(';', pos);
		if (end == std::string::npos)
			end = cookie_header.length();
		
		std::string pair = cookie_header.substr(pos, end - pos);
		size_t eq = pair.find('=');
		if (eq != std::string::npos)
		{
			std::string name = pair.substr(0, eq);
			std::string value = pair.substr(eq + 1);
			
			size_t start = name.find_first_not_of(" \t");
			size_t last = name.find_last_not_of(" \t");
			if (start != std::string::npos && last != std::string::npos)
				name = name.substr(start, last - start + 1);
			cookies[name] = value;
		}
		pos = end + 1;
	}
	return cookies;
}