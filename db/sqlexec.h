/*
 * sqlexec.h
 *
 *  Created on: Jan 25, 2021
 *      Author: user
 */

#ifndef DB_SQLEXEC_H_
#define DB_SQLEXEC_H_


#include <string>
#include "db_table_class_list.h"
#include "../gui/OutputForm.h"
namespace db {

class sql_exec final : private PgConnection, private Log {
	std::string sql;


public:
	sql_exec(const char * new_sql):  Log("sql_exec"), sql(new_sql){
		msg(new_sql);
	}

	std::unique_ptr<gui::OutputForm> showReport();

	virtual ~sql_exec() {

	}
};

} /* namespace db */

#endif /* DB_SQLEXEC_H_ */
