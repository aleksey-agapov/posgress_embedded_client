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


}

#endif /* UTILS_APPUTILS_H_ */