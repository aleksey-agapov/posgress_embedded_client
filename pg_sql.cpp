//============================================================================
// Name        : pg_sql.cpp
// Author      : Boris
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#define POSGRESS

#ifdef POSGRESS

#include <iostream>
#include <pqxx/pqxx>
#include <memory>
#include <utility>
#include <thread>
#include <chrono>

#include "config/AppConfigControl.h"
#include "control/CommandControl.h"
#include "control/consoleinterface.h"
#include "thread/ThreadTest.h"

//extern std::shared_ptr<control::History> history;

using namespace threads;



int main(int argc, char *argv[]) {

	threads::ThreadTest thread_test;
	config::DbAppConfig db_config;

	std::shared_ptr<control::CommandControl> control = std::make_shared<control::CommandControl>();
	int ret_code = 0;
	std::string parms = db_config.getConfigLine();// "dbname = messagedb user = postgres password = 12345678 hostaddr = 127.0.0.1 port = 5432";
	try {
		db::PgConnection connector(parms);
		std::shared_ptr<pqxx::connection> db_connect = connector.getConnection();
		if (db_connect->is_open()) {
			std::cout << "Opened database successfully: " << db_connect->dbname() << std::endl;
			std::cout << "              server version: " << db_connect->server_version()
					<< std::endl;
			std::cout << "            protocol version: " << db_connect->protocol_version()
					<< std::endl;
		} else {
			std::cout << "Can't open database" << std::endl;
			ret_code = 1;
		}

		init_db_control();

		init_cmd_history(20);


/*
		std::thread t5(&threads::ThreadTest::start_thread, &thread_test); // t5 runs foo::bar() on object f
		control::ptr_action_func run_cmd = std::make_shared<control::intercept_func> ([&](std::string& in){thread_test.resume();});
		control::ptr_action_func stop_cmd = std::make_shared<control::intercept_func> ([&](std::string& in){thread_test.pause();});

		control.RegisterOperation("resume", run_cmd);
		control.RegisterOperation("pause", stop_cmd);
*/

		// std::function
		control::ptr_action_func exit_cmd           = std::make_shared<control::intercept_func> (exit_func);
		control::ptr_action_func clear_cmd          = std::make_shared<control::intercept_func> (clear_func);
		control::ptr_action_func help_cmd           = std::make_shared<control::intercept_func> (help_func);
		control::ptr_action_func db_list_cmd        = std::make_shared<control::intercept_func> (db_list_func);
		control::ptr_action_func db_info_cmd        = std::make_shared<control::intercept_func> (db_info_func);
		control::ptr_action_func db_select_cmd      = std::make_shared<control::intercept_func> (db_select_func);
		control::ptr_action_func db_set_schema_cmd  = std::make_shared<control::intercept_func> (set_schema_func);
		control::ptr_action_func history_cmd        = std::make_shared<control::intercept_func> (show_history);
		control::ptr_action_func execute_sql_cmd    = std::make_shared<control::intercept_func> (execute_sql);

		control::ptr_action_func config_operation_cmd    = std::make_shared<control::intercept_func> (config_operation);

		control->RegisterHistory(get_history(), "history");

		control->RegisterOperation("^C", exit_cmd);
		control->RegisterOperation("exit", exit_cmd);
		control->RegisterOperation("clear", clear_cmd);
		control->RegisterOperation("help", help_cmd);


		control->RegisterOperation("list", db_list_cmd);
		control->RegisterOperation("info", db_info_cmd);
		control->RegisterOperation("show", db_select_cmd);
		control->RegisterOperation("schema", db_set_schema_cmd);
		control->RegisterOperation("history", history_cmd);
		control->RegisterOperation("sql", execute_sql_cmd);
		control->RegisterOperation("config", config_operation_cmd);

		control->RegisterDefault(execute_sql_cmd);

		control->Start();
	} catch (pqxx::sql_error const &e) {
		std::cerr << "Database error: " << e.what() << std::endl
				<< "Query was: " << e.query() << std::endl;
		ret_code = -2;
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		ret_code = -1;
	}

	control->Stop();
	return ret_code;
}
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
