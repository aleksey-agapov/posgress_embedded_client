/*
 * selector.h
 *
 *  Created on: Dec 28, 2020
 *      Author: user
 */

#ifndef DB_SELECTOR_H_
#define DB_SELECTOR_H_
#include <sstream>
#include <string>
#include "db_table_class_list.h"
#include "../gui/OutputForm.h"
namespace db {

/*
 *
 *
 *
    [ LIMIT { число | ALL } ]
    [ OFFSET начало [ ROW | ROWS ] ]
 */
class selector : protected PgConnection {
	vector<std::shared_ptr<InterfaceTable> > table_list;
	std::stringstream select_sql;
	std::stringstream fields;
	std::stringstream from;
	std::stringstream where;
	std::stringstream range;

	int isFoundSelectTable(int select_count, const std::string & schema, const std::string & table);
	void calc_column(int count);
	bool calculateDateRange(int position, int size);

public:
	selector () = delete;
	selector(vector<std::shared_ptr<InterfaceTable> > new_table_list);
	selector(const selector &other) = delete;
	selector(selector &&other) = delete;
	virtual ~selector() = default;

	selector& operator=(const selector &other) = delete;
	selector& operator=(selector &&other) = delete;
	std::unique_ptr<gui::OutputForm> showReport(int position, int size);
};

} /* namespace db */

#endif /* DB_SELECTOR_H_ */
