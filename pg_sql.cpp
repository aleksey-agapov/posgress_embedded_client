//============================================================================
// Name        : pg_sql.cpp
// Author      : Boris
// Version     :
// Copyright   : Your copyright notice
// Description : PostgreSQL C++, Ansi-style
//============================================================================
#define POSGRESS

#ifdef POSGRESS

#include <iostream>
#include <pqxx/pqxx>
#include <memory>
#include <utility>
//#include <thread>
#include <chrono>

#include "config/AppConfigControl.h"
#include "control/CommandControl.h"
#include "control/consoleinterface.h"
#include "control/consolInterfaceCmd.h"

// "dbname = messagedb user = postgres password = 12345678 hostaddr = 127.0.0.1 port = 5432";
int main(int argc, char *argv[]) {
	int ret_code = 0;
	std::shared_ptr<config::DbAppConfig> db_config;
	std::shared_ptr<control::CommandControl> control;
	std::shared_ptr<db::PgConnection> connector;
	const char * list_of_priority_cmd[]  = {SET, EXIT1, EXIT2, HELP, nullptr};

	try {
		init_config_control();
		db_config = get_db_config_control();
		control = std::make_shared<control::CommandControl>();
		connector = std::make_shared<db::PgConnection>(db_config->getConfigLine());
		connector->setConfigControl(db_config);

		init_about_control(ABOUT_TITLE);
		init_db_control();
		init_cmd_history(20);
		init_default_control();

		// std::function
		control::ptr_action_func exit_cmd             = std::make_shared<control::intercept_func> (exit_func);
		control::ptr_action_func clear_cmd            = std::make_shared<control::intercept_func> (clear_func);
		control::ptr_action_func help_cmd             = std::make_shared<control::intercept_func> (help_func);
		control::ptr_action_func db_list_cmd          = std::make_shared<control::intercept_func> (db_list_func);
		control::ptr_action_func db_info_cmd          = std::make_shared<control::intercept_func> (db_info_func);
		control::ptr_action_func db_select_cmd        = std::make_shared<control::intercept_func> (db_select_func);
		control::ptr_action_func db_set_schema_cmd    = std::make_shared<control::intercept_func> (set_schema_func);
		control::ptr_action_func history_cmd          = std::make_shared<control::intercept_func> (show_history);
		control::ptr_action_func execute_sql_cmd      = std::make_shared<control::intercept_func> (execute_sql);

		control::ptr_action_func config_operation_cmd = std::make_shared<control::intercept_func> (config_operation);
		control::ptr_action_func set_default_cmd      = std::make_shared<control::intercept_func> (set_default);

		control->RegisterAbout(get_about_control());
		control->RegisterHistory(get_history(), HISTORY);
		control->RegisterSetDefault(get_set_default(), list_of_priority_cmd);


		control->RegisterOperation(EXIT1, exit_cmd, false, EMPTY);
		control->RegisterOperation(EXIT2, exit_cmd, false, EXIT_INFO);
		control->RegisterOperation(CLEAR, clear_cmd, false, CLEAR_INFO);
		control->RegisterOperation(HELP, help_cmd, true, HELP_INFO);


		control->RegisterOperation(LIST, db_list_cmd, false, LIST_INFO);
		control->RegisterOperation(INFO, db_info_cmd, true, INFO_INFO);
		control->RegisterOperation(SHOW, db_select_cmd, true, SHOW_INFO);
		control->RegisterOperation(SCHEMA, db_set_schema_cmd, true, SCHEMA_INFO);
		control->RegisterOperation(HISTORY, history_cmd, true, HISTORY_INFO);
		control->RegisterOperation(SQL, execute_sql_cmd, true, SQL_INFO);
		control->RegisterOperation(CONFIG, config_operation_cmd, true, CONFIG_INFO);
		control->RegisterOperation(SET, set_default_cmd, false, SET_INFO);

		control->RegisterDefault(execute_sql_cmd);
	} catch (const std::exception &e) {
		std::cerr << "Error:"<< e.what() << std::endl;
		std::exit(-1);
	}

	try {
		control->Start();
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		ret_code = -2;
	}
	return ret_code;
}

/*
 * 		threads::ThreadTest thread_test;
		std::thread t5(&threads::ThreadTest::start_thread, &thread_test); // t5 runs foo::bar() on object f
		control::ptr_action_func run_cmd = std::make_shared<control::intercept_func> ([&](std::string& in){thread_test.resume();});
		control::ptr_action_func stop_cmd = std::make_shared<control::intercept_func> ([&](std::string& in){thread_test.pause();});

		control.RegisterOperation("resume", run_cmd);
		control.RegisterOperation("pause", stop_cmd);
*
*/





#else

#include <string>
#include <iostream>
#include <sstream>
#include <functional>

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


int main()
{
    // greet the user
    std::string name;
    std::cout << "What is your name? ";
    std::getline(std::cin, name);
    trim(name);
    std::cout << "Hello " << name << ", nice to meet you.\n";

    // read file line by line
    std::istringstream input;
    input.str("1\n2\n3\n4\n5\n6\n7\n");
    int sum = 0;
    for (std::string line; std::getline(input, line); ) {
        sum += std::stoi(line);
    }
    std::cout << "\nThe sum is: " << sum << "\n";
}
#endif
