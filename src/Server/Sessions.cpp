#include "Sessions.hpp"

Sessions::Sessions()
{
	srand(time(0));
}

Sessions::~Sessions(){}

std::string Sessions::createSession()
{
	std::ostringstream res;
	res << "session_" << rand() << "gaeudes" << rand();
	std::string session_id = res.str();

	_sessions[session_id] = std::map<std::string, std::string>();
	std::cout << "\033[1;32mSession created: " << session_id << "\033[0m" << std::endl;
	return session_id;
}

bool Sessions::exists(const std::string &session_id) const
{
	bool found = _sessions.find(session_id) != _sessions.end();
	return found;
}

void Sessions::set(const std::string &session_id, const std::string &key, const std::string &value)
{
	_sessions[session_id][key] = value;
}

std::string Sessions::get(const std::string &session_id, const std::string &key) const
{
	std::map<std::string, std::map<std::string, std::string> >::const_iterator it = _sessions.find(session_id);
	if (it != _sessions.end())
	{
		std::map<std::string, std::string>::const_iterator key_it = it->second.find(key);
		if (key_it != it->second.end())
			return key_it->second;
	}
	return "";
}

void Sessions::erase(const std::string &session_id)
{
	_sessions.erase(session_id);
}
