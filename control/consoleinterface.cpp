/*
 * consoleinterface.cpp
 *
 *  Created on: Nov 15, 2020
 *      Author: Agapov Aleksey
 */


#include <iostream>
#include <tuple>
#include <algorithm>
#include <cctype>
#include <string>
#include <thread>

#include "consoleinterface.h"
#include "CommandControl.h"
#include "About.h"
#include "DefaultSchema.h"
#include "History.h"

#include "../utils/apputils.h"
#include "../db/db_table_class_list.h"
#include "../db/selector.h"
#include "../db/sqlexec.h"
#include "../thread/log_thread.h"



std::shared_ptr<db::TabelControl> db_control;
std::shared_ptr<control::History> history;
std::shared_ptr<control::DefaultSchema> default_control;
std::shared_ptr<config::DbAppConfig> db_config;
std::shared_ptr<config::LogAppConfig> log_config;
std::shared_ptr<control::About> about_module;
std::shared_ptr<threads::LogThread> logThread;

void init_about_control(const char * title){
	about_module = std::make_shared<control::About> (title);
}


std::shared_ptr<control::About> get_about_control(){
	return about_module;
}

// ////////////////////////////////////////////////////////////////


void init_config_control(){
	db_config = std::make_shared<config::DbAppConfig> ();
	log_config= std::make_shared<config::LogAppConfig> ();
	logThread = std::make_shared<threads::LogThread>(log_config->getLogFilePath());
}

std::shared_ptr<config::LogAppConfig> get_log_config_control(){
	return log_config;
}

std::shared_ptr<config::DbAppConfig> get_db_config_control(){
	return db_config;
}

std::shared_ptr<threads::LogThread> get_log_deamon(){
	return logThread;
}




/*
 * edit
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
		command_string = cmd_line.substr(first_pos + 1);
		command = cmd_line.substr(0, first_pos);
	}

	if (!command_string.empty()) {
		if (command.compare("load") == 0) {
			db_config->LoadNewConfig(command_string.c_str());
		} else
		if (command.compare("save") == 0) {
			db_config->SaveConfig(command_string.c_str());
		} else
		if (command.compare("edit") == 0) {
			first_pos = command_string.find(delimiter);
			if (first_pos != std::string::npos) {
				std::string obj_value = command_string.substr(first_pos + 1);
				std::string obj_key   = command_string.substr(0, first_pos);
				db_config->setCommandLineValue(obj_key.c_str(), obj_value.c_str());
			}
		}
	} else {
		if (command.compare("clear") == 0) {
			db_config->clear();
		} else
		if (command.compare("show") == 0) {
			std::cout << *db_config->showTree(db_config->getModuleName()) << std::endl;
		}
	}
}
/*
 * edit
 * show
 * clear
 */
void log_operation (std::string& cmd_line) {
	const std::string delimiter( DELIMITER);
	std::string command_string;
	std::string command;
	std::string::size_type first_pos = cmd_line.find(delimiter);
	if (first_pos == std::string::npos) {
		command = cmd_line;
	} else {
		command_string = cmd_line.substr(first_pos + 1);
		command = cmd_line.substr(0, first_pos);
	}

	if (!command_string.empty()) {
		if (command.compare("edit") == 0) {
			first_pos = command_string.find(delimiter);
			if (first_pos != std::string::npos) {
				std::string obj_value = command_string.substr(first_pos + 1);
				std::string obj_key   = command_string.substr(0, first_pos);
				if (log_config->setCommandLineValue(obj_key.c_str(), obj_value.c_str())) {
					logThread->Update();
				}
			}
		}
	} else {
		if (command.compare("clear") == 0) {
			log_config->clear();
		} else
		if (command.compare("show") == 0) {
			std::cout << *log_config->showTree(log_config->getModuleName()) << std::endl;
		}
	}
}

/*
 * schema
 */
void set_schema_func(std::string& in) {
	db_control->setSchema(in);
}

/*
 * list tables
 */
void db_list_func (std::string& in){
	clear_func(in);
	std::unique_ptr<gui::OutputForm> table_list = db::reportTablesList( db_control->getTablesList());
	table_list->setTitle("List tables");
	gui::printer show_list(std::cout);
	show_list.showForm(std::move(table_list));
}

/*
 * table info
 */
void db_info_func (std::string& in){
	if (in.empty()) return;
	std::shared_ptr<db::InterfaceTable> table;
	if (utils::is_number(in)) {
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

		utils::trim(schema);
		utils::trim(table_name);

		if (schema.empty()) {table = db_control->getTableInfo(table_name, false);}
		else {table = db_control->getTableInfo(schema, table_name, false);}
	}
	if (table){
		std::unique_ptr<gui::OutputForm> table_info = db::reportTablesInfo(table->getHeaderInfo());
		table_info->setTitle("Table info:"+table->getSchema()+"."+table->getName());
		gui::printer show_list(std::cout);
		show_list.showForm(std::move(table_info));
	}
}


/*
 * show table data
 */
void db_select_func (std::string& in){
	if (in.empty()) return;
	int show_pos = 0;
	int show_size = 10;

	std::string opt_delimiter (OPTIONS_DELIMITER);
	std::string delimiter ( DELIMITER );
/////////////
	std::string::size_type opt_pos = in.find(opt_delimiter);
	if (opt_pos != std::string::npos) {
		std::string option_str = in.substr(opt_pos + 1);
		in.erase(opt_pos);

		if ((opt_pos = option_str.find(delimiter)) != std::string::npos) {
			show_pos  = utils::getInteger(option_str.substr(0, opt_pos), show_pos);
			option_str.erase(0, opt_pos + delimiter.length());
			show_size = utils::getInteger(option_str, show_size);
		} else {
			show_pos  = utils::getInteger(option_str, show_pos);
		}
	}
////////
	std::vector<int> table_list;
	auto add_list_func = [&](std::string in_val) {
		utils::trim(in_val);
		if (utils::is_number(in_val)) {
			table_list.push_back(std::stoi(in));
		}
	};

	size_t pos = 0;
	while ((pos = in.find(delimiter)) != std::string::npos) {
		add_list_func( in.substr(0, pos));
		in.erase(0, pos + delimiter.length());
	}
	add_list_func(in);

	std::unique_ptr<db::selector> table_request = db_control->getTablesShow(table_list);

	gui::printer show_list(std::cout);
	show_list.showForm(std::move(table_request->showReport(show_pos, show_size)));
}


/*
 * refresh table list
 */
void refresh_table_list (std::string& in){
	db_control->getTablesList(true);
}

/*
 * reconnect to db
 */
void reconnect (std::string& in){
	db_control->Reconnect();
}


// /////////////////////////////////

void set_default(std::string& in) {
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
	if (utils::is_number(in)) {
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
		std::cerr << "EqlExecute Error:" << ex.what() << std::endl;
	}
}




void init_db_control() {
	db_control = std::make_shared<db::TabelControl> ();
}

void exit_func(std::string& in) {
	logThread->Destroy();
	std::cout << in << " EXIT!!!" << std::endl;
	exit(0);
}

void clear_func (std::string& in) {
	// CSI[2J clears screen, CSI[H moves the cursor to top-left corner
	std::cout << "\x1B[2J\x1B[H\033c";
}

void help_func (std::string& in) {
	clear_func(in);
	about_module->printAbout(std::cout);
}

