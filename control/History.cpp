/*
 * History.cpp
 *
 *  Created on: Jan 22, 2021
 *      Author: Agapov Aleksey
 */

#include "HistoryException.h"
#include "History.h"
#include "../gui/OutputForm.h"
#include "CommandControl.h"

namespace control {
void History::ShowValue() {
	std::unique_ptr<gui::OutputForm> history_report(new gui::OutputForm());

	history_report->setTitle("HISTORY");
	history_report->add_column("number", gui::OutputForm::ColumnType::NUM).add_column("command");
	if (!qhistory.empty()) {
		int history_count = 0;
		for (std::deque<std::string>::iterator history_pointer = std::begin(qhistory); history_pointer != std::end(qhistory);history_pointer++) {
			history_report->add_row( { std::to_string(history_count++), history_pointer->c_str() });
		}
	}

	gui::printer show_list(std::cout);
	show_list.showForm(std::move(history_report));
}

bool History::addValue(std::string input) {
	bool result_code = true;
	try {
		if (!isFound(input)) {
			qhistory.push_back(input);
			if (qhistory.size() > qhistory_size) {
				qhistory.pop_front();
			}
		}
	} catch (...) {
		result_code = false;
	}
	return result_code;
}

void History::runValue(int indexValue) {
	if (static_cast<std::deque<std::string>::size_type>(indexValue)
			< qhistory.size()) {
		cmd_interface->run_cmd( qhistory[indexValue], true);
	} else {
		std::cerr << "Error found command in History" << std::endl;
	}
}

void History::add_cmd_interface(std::shared_ptr<RunCmdInterface> new_cmd_interface)  {
	cmd_interface = new_cmd_interface;
}

bool History::isFound(std::string& new_value){
	bool is_Found = false;
	for (std::string item : qhistory) {
		if (item.compare(new_value) == 0) {is_Found = true; break;}
	}
	return is_Found;
}


}
