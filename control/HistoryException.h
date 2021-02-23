/*
 * HistoryException.h
 *
 *  Created on: Jan 21, 2021
 *      Author: user
 */

#ifndef CONTROL_HISTORY_EXCEPTION_H_
#define CONTROL_HISTORY_EXCEPTION_H_

#include <exception>
#include <string>

class HistoryException : public std::exception {
	std::string err_msg;
	public:
	HistoryException(std::string error) : err_msg(error) {}
	virtual ~HistoryException() = default;

	const char* what() const noexcept { return err_msg.c_str(); } // C++11 и выше
};




#endif /* CONTROL_HISTORY_H_ */
