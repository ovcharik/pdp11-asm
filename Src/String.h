#pragma once

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

static inline std::string &ltrim(std::string &s);
static inline std::string &rtrim(std::string &s);
static inline std::string &trim(std::string &s);


class String
{
public:
	// trim from start
	static std::string &ltrim(std::string &s)
	{
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
		return s;
	}

	// trim from end
	static std::string &rtrim(std::string &s)
	{
		s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
		return s;
	}

	// trim from both ends
	static std::string &trim(std::string &s)
	{
		return ltrim(rtrim(s));
	}

	static std::string &to_upper(std::string &s)
	{
		std::transform(s.begin(), s.end(), s.begin(), std::toupper);
		return s;
	}

	static std::string &to_lower(std::string &s)
	{
		std::transform(s.begin(), s.end(), s.begin(), std::tolower);
		return s;
	}
};