/*
 * consoleinterface.h
 *
 *  Created on: Nov 15, 2020
 *      Author: Agapov Aleksey
 */

#ifndef CONTROL_CONSOLEINTERFACE_H_
#define CONTROL_CONSOLEINTERFACE_H_


#include <string>
#include <memory>


namespace threads {
	class LogThread;
}

namespace config {
	class DbAppConfig;
	class LogAppConfig;
}


namespace control {
	class About;
	class History;
	class DefaultSchema;
}


using namespace std;

extern void exit_func(std::string& in);
extern void clear_func (std::string& in);
extern void help_func (std::string& in);
extern void init_db_control();
extern void db_list_func (std::string& in);
extern void db_info_func (std::string& in);
extern void db_select_func (std::string& in);
extern void set_schema_func (std::string& in);

extern std::shared_ptr<control::History> get_history();
extern void init_cmd_history(int history_size);
extern void show_history(std::string& in);
extern void execute_sql (std::string& in);


extern void init_config_control();
extern std::shared_ptr<config::DbAppConfig> get_db_config_control();
extern std::shared_ptr<config::LogAppConfig> get_log_config_control();
extern std::shared_ptr<threads::LogThread> get_log_deamon();
extern void config_operation (std::string& in);
extern void log_operation (std::string& in);

extern void init_default_control();
extern void set_default(std::string& in);
extern std::shared_ptr<control::DefaultSchema>get_set_default();

extern void init_about_control(const char * title);
extern std::shared_ptr<control::About> get_about_control();

extern void refresh_table_list (std::string& in);
extern void reconnect (std::string& in);


#endif /* CONTROL_CONSOLEINTERFACE_H_ */
