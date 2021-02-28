/*
 * sqlexec.cpp
 *
 *  Created on: Jan 25, 2021
 *      Author: Agapov Aleksey
 */

#include "sqlexec.h"

namespace db {

std::unique_ptr<gui::OutputForm> sql_exec::showReport(){
	std::unique_ptr<gui::OutputForm> report(new gui::OutputForm());

	if (isReady()) {
		pqxx::work statement{*getConnection()};
		pqxx::result ret_data { statement.exec(sql) };

		ssize_t column_size = ret_data.columns();
	/*
	 *
	 */
		for (ssize_t column_index = 0; column_index < column_size; column_index++){
			ColumnType columnType = getType(ret_data.column_type(static_cast<int>(column_index)));
			report->add_column(ret_data.column_name(column_index), columnType);
		}

		vector<std::string> new_row;
		for (auto row: ret_data) {
			new_row.clear();
			for (ssize_t column_index = 0; column_index < column_size; column_index++){
				new_row.push_back(
						row[static_cast<int>(column_index)].is_null()?
								"":
						row[static_cast<int>(column_index)].as<std::string>()
				);
			}
			report->add_row(new_row);
		}
	} else {
		msg("Error: Connection lost.");
	}
	return std::move(report);
}



} /* namespace db */
