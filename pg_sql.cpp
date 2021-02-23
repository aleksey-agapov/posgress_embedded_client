//============================================================================
// Name        : pg_sql.cpp
// Author      : Boris
// Version     :
// Copyright   : Your copyright notice
// Description : PostgreSQL C++, Ansi-style
//============================================================================

#include <iostream>
#include <memory>
#include <utility>

#include "db/db_table_class_list.h"
#include "thread/log_thread.h"
#include "config/AppConfigControl.h"
#include "control/CommandControl.h"
#include "control/consoleinterface.h"
#include "control/consolInterfaceCmd.h"

// "dbname = messagedb user = postgres password = 12345678 hostaddr = 127.0.0.1 port = 5432";
int main(int argc, char *argv[]) {
	std::shared_ptr<threads::LogThread> logThread;
	std::shared_ptr<config::DbAppConfig>  db_config_module;
	std::shared_ptr<config::LogAppConfig> log_config_module;
	std::shared_ptr<control::CommandControl> control;
	std::shared_ptr<db::PgConnection> connector;
	const char * list_of_priority_cmd[]  = {SET, EXIT1, EXIT2, HELP, nullptr};

	try {
		init_config_control();
		db_config_module = get_db_config_control();
		log_config_module = get_log_config_control();

		logThread = get_log_deamon();

		control = std::make_shared<control::CommandControl>();
		connector = std::make_shared<db::PgConnection>();
		connector->setConfigControl(db_config_module);

		logThread->setConfigControl(log_config_module);
		logThread->start_loger();

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
		control::ptr_action_func log_operation_cmd    = std::make_shared<control::intercept_func> (log_operation);
		control::ptr_action_func set_default_cmd      = std::make_shared<control::intercept_func> (set_default);

		control->RegisterAbout(get_about_control());
		control->RegisterHistory(get_history(), HISTORY);
		control->RegisterSetDefault(get_set_default(), list_of_priority_cmd);

		control->RegisterOperation(EXIT1,   exit_cmd, false, EMPTY);
		control->RegisterOperation(EXIT2,   exit_cmd, false, EXIT_INFO);
		control->RegisterOperation(CLEAR,   clear_cmd, false, CLEAR_INFO);
		control->RegisterOperation(HELP,    help_cmd, false, HELP_INFO);

		control->RegisterOperation(LIST,    db_list_cmd, false, LIST_INFO);
		control->RegisterOperation(INFO,    db_info_cmd, true, INFO_INFO);
		control->RegisterOperation(SHOW,    db_select_cmd, true, SHOW_INFO);
		control->RegisterOperation(SCHEMA,  db_set_schema_cmd, false, SCHEMA_INFO);
		control->RegisterOperation(HISTORY, history_cmd, true, HISTORY_INFO);
		control->RegisterOperation(SQL,     execute_sql_cmd, true, SQL_INFO);
		control->RegisterOperation(CONFIG,  config_operation_cmd, true, CONFIG_INFO);

		control->RegisterOperation(LOG,     log_operation_cmd, true, LOG_INFO);
		control->RegisterOperation(SET,     set_default_cmd, false, SET_INFO);

		control->RegisterDefault(execute_sql_cmd);
	} catch (const std::exception &e) {
		std::cerr << "Error of initialization:"<< e.what() << std::endl;
		std::exit(-1);
	}

	try {
		control->Start();
	} catch (const std::exception &e) {
		std::cerr << "Critical error:" << e.what() << std::endl;
		std::exit(-2);
	}
	std::exit(0);
}
