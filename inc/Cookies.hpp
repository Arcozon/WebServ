#ifndef COOKIES_HPP
#define COOKIES_HPP

#include <string>
#include <ostream>
#include <map>
#include <iostream>
#include <sstream>

class Cookies
{
private:
	std::string _name;
	std::string _value;
	std::string _path;
	time_t		_duration;
	Cookies(/* args */);
public:
	~Cookies();
	Cookies(const std::string &name, const std::string &value);
	void setPath(const std::string &path);
	void setDuration(int seconds);
	std::string setCookieHeader() const;
	static std::map<std::string, std::string> parseCookie(const std::string &cookie_header);

};



#endif