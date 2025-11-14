#ifndef SESSIONS_HPP
#define SESSIONS_HPP

#include <map>
#include <iostream>
#include <sstream>
#include <stdlib.h>

class Sessions
{
private:
	 std::map<std::string, std::map<std::string, std::string> > _sessions;

public:
	Sessions(/* args */);
	~Sessions();
	std::string createSession();
	bool exists(const std::string &session_id) const;
	void set(const std::string &session_id, const std::string &key, const std::string &value);
	std::string get(const std::string &session_id, const std::string &key) const;
	void erase(const std::string &session_id);
};


#endif