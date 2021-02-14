/*
 * consoleinterface.cpp
 *
 *  Created on: Nov 15, 2020
 *      Author: root
 */

#include <string>
#include <iostream>
#include <tuple>
#include <algorithm>
#include <cctype>
#include <string>
#include <memory>
#include "consoleinterface.h"
#include "CommandControl.h"
#include "About.h"
#include "../utils/apputils.h"


std::shared_ptr<db::TabelControl> db_control;
std::shared_ptr<control::History> history;
std::shared_ptr<control::DefaultSchema> default_control;
std::shared_ptr<config::DbAppConfig> db_config;
std::shared_ptr<control::About> about_module;

void init_about_control(const char * title){
	about_module = std::make_shared<control::About> (title);
}


std::shared_ptr<control::About> get_about_control(){
	return about_module;
}

// ////////////////////////////////////////////////////////////////

void init_config_control(){
	db_config = std::make_shared<config::DbAppConfig> ();
}


std::shared_ptr<config::DbAppConfig> get_db_config_control(){
	return db_config;
}
/*
 *
 * save
 * load
 * show
 * clear
 */
void config_operation (std::string& cmd_line){
	const std::string delimiter( DELIMITER);
	std::string command_string;
	std::string command;
	std::string::size_type first_pos = cmd_line.find(delimiter);
	if (first_pos == std::string::npos) {
		command = cmd_line;
	} else {
		command_string = cmd_line.substr(first_pos + 1); // token is "scott"
		command = cmd_line.substr(0, first_pos); // token is "scott"
	}

	if (!command_string.empty()) {
		if (command.compare("load") == 0) {
			db_config->LoadNewConfig(command_string.c_str());
		} else
		if (command.compare("save") == 0) {
			db_config->SaveConfig(command_string.c_str());
		} else
		/*
		if (command.compare("select") == 0) {
			const char * KeyName = command_string.c_str();
			if (db_config->isContainModule(KeyName)) {
				select_config_module = command_string;
			}
		} else

		if (command.compare("show") == 0) {
			const char * KeyName = command_string.c_str();
			if (db_config->isContainModule(KeyName)) {
				db_config->showTree(KeyName);
			}
		} else
		*/
		if (command.compare("edit") == 0) {
			first_pos = command_string.find(delimiter);
			if (first_pos != std::string::npos) {
				std::string obj_value = command_string.substr(first_pos + 1); // token is "scott"
				std::string obj_key   = command_string.substr(0, first_pos); // token is "scott"
				db_config->setConfigLine(obj_key.c_str(), obj_value.c_str());
			}
		}
	} else {
		if (command.compare("clear") == 0) {
			db_config->clear();
		} else
		if (command.compare("show") == 0) {
			std::cout << *db_config->showTree() << std::endl;
		}
	}


}

bool is_number(const std::string &s) {
  return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}


void set_schema_func(std::string& in) {
	db_control->setSchema(in);
}


void db_list_func (std::string& in){
	clear_func(in);
	std::unique_ptr<gui::OutputForm> table_list = db::reportTablesList( db_control->getTablesList());
	table_list->setTitle("List tables");
	gui::printer show_list(std::cout);
	show_list.showForm(std::move(table_list));
}


void db_info_func (std::string& in){
	std::shared_ptr<db::InterfaceTable> table;
	if (is_number(in)) {
		table = db_control->getTableInfo(std::stoi(in), false);
	} else {
		std::string schema;
		std::string table_name;
		const std::string delimiter( DELIMITER);

		size_t pos = 0;
		while ((pos = in.find(delimiter)) != std::string::npos) {
			schema = in.substr(0, pos);
		    in.erase(0, pos + delimiter.length());
		}
		table_name = in;

		if (schema.empty()) {table = db_control->getTableInfo(table_name, false);}
		else {table = db_control->getTableInfo(schema, table_name, false);}
	}
	std::unique_ptr<gui::OutputForm> table_info = db::reportTablesInfo(table->getHeaderInfo());
	table_info->setTitle("Table info:"+table->getSchema()+"."+table->getName());
	gui::printer show_list(std::cout);
	show_list.showForm(std::move(table_info));
}



void db_select_func (std::string& in){
	int show_pos = 0;
	int show_size = 10;

	std::string opt_delimiter (OPTIONS_DELIMITER);
	std::string delimiter ( DELIMITER );
/////////////
	std::string::size_type opt_pos = in.find(opt_delimiter);
	if (opt_pos != std::string::npos) {
		std::string option_str = in.substr(opt_pos + 1); // token is "scott"
		in.erase(opt_pos);

		if ((opt_pos = option_str.find(delimiter)) != std::string::npos) {
			show_pos  = std::stoi(option_str.substr(0, opt_pos));
			option_str.erase(0, opt_pos + delimiter.length());
			show_size = std::stoi(option_str);
		} else {
			show_pos  = std::stoi(option_str);
		}
	}
////////

	std::vector<int> table_list;
	auto add_list_func = [&](std::string in_val) {
		if (is_number(in_val)) {
			table_list.push_back(std::stoi(in));
		}
	};

	size_t pos = 0;
	while ((pos = in.find(delimiter)) != std::string::npos) {
		add_list_func( in.substr(0, pos));
	    in.erase(0, pos + delimiter.length());
	}
	add_list_func( in);

	std::unique_ptr<db::selector> table_request = db_control->getTablesShow(table_list);

	gui::printer show_list(std::cout);
	show_list.showForm(std::move(table_request->showReport(show_pos, show_size)));

}



// /////////////////////////////////

void set_default(std::string& in) {
	utils::trim(in);
	if (!default_control->setDefault(in)) {
		std::cerr << "Error set new module:" << in << std::endl;
	}
}

void init_default_control() {
	default_control =  std::make_shared<control::DefaultSchema>();
}

std::shared_ptr<control::DefaultSchema>get_set_default() {
	return default_control;
}

// /////////////////////////////////



void init_cmd_history(int history_size) {
	history = std::make_shared<control::History> (history_size);
}

std::shared_ptr<control::History> get_history() {
	return history;
}



void show_history(std::string& in) {
	if (is_number(in)) {
		history->runValue(std::stoi(in));
	} else {
		history->ShowValue();
	}
}

void execute_sql (std::string& in) {
	try {
	db::sql_exec sql_request(in.c_str());
	gui::printer show_list(std::cout);

	std::unique_ptr<gui::OutputForm> sql_report = std::move(sql_request.showReport());
	if (sql_report->getRowSize() > 0) {
		show_list.showForm(std::move(sql_report));
	}
	} catch (const std::exception &ex) {
		std::cerr << ex.what() << std::endl;
	}
}




void init_db_control() {
	db_control = std::make_shared<db::TabelControl> ();
}

void exit_func(std::string& in) {
	std::cout << in << " EXIT!!!" << std::endl;
	exit(0);
}

void clear_func (std::string& in) {
	// CSI[2J clears screen, CSI[H moves the cursor to top-left corner
	std::cout << "\x1B[2J\x1B[H\033c";
//	clrscr();
}

void help_func (std::string& in) {
	clear_func(in);
	about_module->printAbout(std::cout);
}
//} /* namespace control */









