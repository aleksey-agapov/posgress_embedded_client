/*
 * History.h
 *
 *  Created on: Jan 21, 2021
 *      Author: user
 */

#ifndef CONTROL_HISTORY_H_
#define CONTROL_HISTORY_H_

#include <string>
#include <deque>  // подключили библиотеку queue
#include <memory>

#define DEFAULT_HISTORY_SIZE 10

namespace control {
class RunCmdInterface;

class History {
	std::deque<std::string> qhistory;
	std::deque<std::string>::size_type qhistory_size;
	std::shared_ptr<RunCmdInterface> cmd_interface;

	bool isFound(std::string& new_value);

public:
	History() :
		History(DEFAULT_HISTORY_SIZE) {
	}

	History(int hist_size) : cmd_interface(nullptr){
		qhistory_size = static_cast<std::deque<std::string>::size_type>(hist_size);
	}
	virtual ~History() {
	}

	bool addValue(std::string input);

	void runValue(int indexValue);

	void ShowValue();
// std::cout

	void add_cmd_interface(std::shared_ptr<RunCmdInterface> new_cmd_interface);

};

}

#endif /* CONTROL_HISTORY_H_ */
