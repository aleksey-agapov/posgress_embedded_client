/*
 * apputils.h
 *
 *  Created on: Feb 12, 2021
 *      Author: user
 */

#ifndef UTILS_APPUTILS_H_
#define UTILS_APPUTILS_H_

#include <functional>
#include <string>
#include <sstream>
#include <ios>

/*
 * DELIMITERS for parse arg strings
 */
#define DELIMITER " "
#define OPTIONS_DELIMITER "|"

namespace utils {

	// trim from start (in place)
	static inline void ltrim(std::string &s) {
	    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
	        return !std::isspace(ch);
	    }));
	}

	// trim from end (in place)
	static inline void rtrim(std::string &s) {
	    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
	        return !std::isspace(ch);
	    }).base(), s.end());
	}

	// trim from both ends (in place)
	static inline void trim(std::string &s) {
	    ltrim(s);
	    rtrim(s);
	}
	// to lower case
	static inline std::string& to_lower(std::string& str)
	{
	    std::transform(str.begin(), str.end(), str.begin(), static_cast<int(*)(int)>(std::tolower));
	    return str;
	}

	// to upper case
	static inline std::string& to_upper(std::string& str)
	{
	    std::transform(str.begin(), str.end(), str.begin(), static_cast<int(*)(int)>(std::toupper));
	    return str;
	}

	static inline bool to_bool(std::string str) {
	    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	    std::istringstream is(str);
	    bool b;
	    is >> std::boolalpha >> b;
	    return b;
	}

	static inline bool is_number(const std::string &s) {
	  return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
	}

static inline int getInteger(const std::string &s, const int defaultValue) {
	try {
		std::string buff_int_string(s);
		trim(buff_int_string);

		if (!buff_int_string.empty() && is_number(buff_int_string)) {
			return std::stoi(buff_int_string);
		} else {
			return defaultValue;
		}
	} catch (...) {
		return defaultValue;
	}
}



}

#endif /* UTILS_APPUTILS_H_ */
