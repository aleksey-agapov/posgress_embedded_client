/*
 * consoleinterface.h
 *
 *  Created on: Nov 15, 2020
 *      Author: root
 */

#ifndef CONTROL_CONSOLEINTERFACE_H_
#define CONTROL_CONSOLEINTERFACE_H_\

using namespace std;

#include <iostream>
#include "../db/db_table_class_list.h"
#include "../db/selector.h"
#include "../db/sqlexec.h"
#include "History.h"

/*
 * DELIMITERS for parse arg strings
 */
#define DELIMITER " "
#define OPTIONS_DELIMITER "|"


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


extern void config_operation (std::string& in);

#endif /* CONTROL_CONSOLEINTERFACE_H_ */
