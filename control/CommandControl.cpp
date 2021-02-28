/*
 * CommandControl.cpp
 *
 *  Created on: Feb 13, 2021
 *      Author: Agapov Aleksey
 */

#include <pqxx/pqxx>
#include "../utils/apputils.h"
#include "History.h"
#include "About.h"
#include "DefaultSchema.h"
#include "CommandControl.h"

namespace control {

CommandControl::CommandControl(): control::Log("command-control"), default_func(nullptr), ignoreDefaultCmdList(nullptr) {}


void CommandControl::Start(){
	std::string cmd_line;
	while (true) {
		if (std::cin.fail()) // если предыдущее извлечение было неудачным,
		{
		    std::cin.clear(); // то возвращаем cin в 'обычный' режим работы
		    std::cin.ignore(32767,'\n'); // и удаляем значения предыдущего ввода из входного буфера
		}

		std::string default_module = defaultClass->getDefaultModule();
		if (default_module.empty()) {
			std::cout << "Read cmd>>";
		} else {
			std::cout << "Read cmd>" << default_module << ">";
		}
		std::getline(std::cin, cmd_line);
		utils::trim(cmd_line);

		if (!cmd_line.empty()) {
			try {
				run_cmd(cmd_line, false);
			} catch (pqxx::sql_error const &e) {
				msg( "Database error: " , e.what() , ". Query was: " , e.query() );
			}
		}
	}
}

void CommandControl::RegisterAbout(std::shared_ptr<control::About> about_class){
	msg("register about class");
	aboutClass = about_class;
}


void CommandControl::RegisterSetDefault(std::shared_ptr<control::DefaultSchema> default_class, const char * ignoreDefaultCmd[]) {
	this->defaultClass = default_class;
	this->ignoreDefaultCmdList = ignoreDefaultCmd;
	std::shared_ptr<control::IsDefaultCmdInterface> pp = std::shared_ptr<control::IsDefaultCmdInterface>(this);
	this->defaultClass->setDefaultInterfaceListner(pp);
}


void CommandControl::RegisterHistory(std::shared_ptr<control::History> new_history, std::string history_cmd ) {
	this->history = new_history;
	this->history_token = history_cmd;
	std::shared_ptr<control::RunCmdInterface> pp = std::shared_ptr<control::RunCmdInterface>(this);
	this->history->add_cmd_interface(pp); // shared_from_this()
}


void CommandControl::RegisterOperation(const char * Key, ptr_action_func &ptr_func, bool isDefault, const char ** about_info){
	// TODO Auto-generated constructor stub
	msg("RegisterOperation: " , Key);
	cmd_list[Key] = ptr_func;
	is_default_list[Key] = isDefault;
	aboutClass->AddInfo(Key, about_info);
}

void CommandControl::RegisterDefault(ptr_action_func &ptr_func){
	default_func = ptr_func;
}

void CommandControl::run_cmd(std::string cmd_line, bool isHistory) {
	const std::string delimiter(DELIMITER);
	std::string token;
	std::string command_string;

	std::string::size_type first_pos = cmd_line.find(delimiter);
	if (first_pos == std::string::npos) {
		token = cmd_line;
	} else {
		command_string = cmd_line.substr(first_pos + 1); // token is "scott"
		token = cmd_line.substr(0, first_pos); // token is "scott"
		utils::trim(command_string);
	}

	if (!defaultClass->getDefaultModule().empty()) {
		if (!isInRegisterList(token.c_str())) {
			token = defaultClass->getDefaultModule();
			command_string = cmd_line;
		}
	}

	bool isDefault = true;
	for (const auto &pair : cmd_list) {
		if (strcmp(pair.first,token.c_str()) == 0) {
			(*pair.second)(command_string);
			isDefault = false;
			if (!isHistory ) {
				if (!history_token.empty()  && ( token.find(history_token) == std::string::npos) ) {
					std::ostringstream history_string;
					if (command_string.empty()) {
						history_string << token;
					} else {
						history_string << token << std::string(DELIMITER) << command_string;
					}
					if (!history->addValue(history_string.str())){
						msg( "Error add command ", history_string.str().c_str() ," in History" );
					}
				}
			}
			break;
		}
	}
	if (isDefault && default_func) {
		(*default_func)(cmd_line);
	}
}



bool CommandControl::isInRegisterList(const char * key){
	bool isFound = false;
	const char ** first_index = ignoreDefaultCmdList;
	while (*first_index){
		if (strcmp (*(first_index++), key) == 0 ) {
			isFound = true;
			break;
		}
	}
	return isFound;
}


bool CommandControl::isDefaultSchema(std::string cmd_line) {
	bool isDefault = false;
	try {
		for (std::pair<const char *, bool> record: is_default_list)
		{
			if (strcmp (record.first, cmd_line.c_str()) == 0 ) {
				isDefault = record.second;
			}
		}
	} catch (...){
		msg( "key " , cmd_line , " not found");
	}
	return isDefault;
}




} // namespace control
