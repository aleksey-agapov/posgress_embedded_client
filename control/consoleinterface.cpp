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



std::shared_ptr<db::TabelControl> db_control;
std::shared_ptr<control::History> history;




void config_operation (std::string& in){

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
		std::string delimiter = DELIMITER;

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
	std::unique_ptr<gui::OutputForm> help(new gui::OutputForm());

	help->setTitle("HELP");
	help->add_column("command").add_column("description");

	help->add_row({"clear","Clear screen"});
	help->add_row({"help <option>","This help"});
	help->add_row({"list","List operation"});
	help->add_row({"",""});
	help->add_row({"info","Info about db object"});
	help->add_row({"<option>","<number> - number db object"});
	help->add_row({"","<label> - label db object"});
	help->add_row({"","<schema>.<label> - schema + label db object"});
	help->add_row({"",""});
	help->add_row({"schema","set default schema"});
	help->add_row({"show","show data db object"});
	help->add_row({"exit","Quit"});


	gui::printer show_list(std::cout);
	show_list.showForm(std::move(help));
}
//} /* namespace control */
