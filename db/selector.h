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
class selector : private Log, private PgConnection {
	vector<std::shared_ptr<InterfaceTable> > table_list;
	std::stringstream select_sql;
	std::stringstream fields;
	std::stringstream from;
	std::stringstream where;
	std::stringstream range;

	int isFoundSelectTable(int select_count, const std::string & schema, const std::string & table) {
		int isFound = -1;
		for(int search_index = 0; search_index < static_cast<int>(table_list.size()); search_index++){
			if (search_index!=select_count) {
				if((table_list[search_index]->getSchema().compare(schema)== 0)&&(table_list[search_index]->getName().compare(table)==0)) {
					if(!where.str().empty()) {
						where << " AND " ;
					}
					isFound = search_index;
					break;
				}
			}
		}
		return isFound;
	}


	void calc_column(int count) {

		std::shared_ptr<InterfaceTable> sel_table = table_list[count];
		std::vector<ColumnInfo> HeaderInfo = sel_table->getHeaderInfo();
		std::for_each(std::begin(HeaderInfo), std::end(HeaderInfo), [&](ColumnInfo& column){
			bool isWriteField = true;

			if (column.isFk()) {
				int found = isFoundSelectTable(count, column.getFkSchema(), column.getFkTable());
				if (found > -1) {
					isWriteField = false;
					where <<  " T" << count << "."<< column.getLabel() <<"="<<"T"<<found<<"."<<column.getFkColumn();
				}
			}

			if (isWriteField) {
				if (!fields.str().empty()) {fields << ", ";}
				fields << " T" << count << "."<< column.getLabel();
			}
		});
	}

	bool calculateDateRange(int position, int size){
		range.clear();
		if (position) {
			range << " OFFSET " << position;

		}

		if (size) {
			range << " LIMIT " << size << " ";
		}
		return !range.str().empty();
	}



public:
	selector () = delete;
	selector(vector<std::shared_ptr<InterfaceTable> > new_table_list);
	selector(const selector &other) = delete;
	selector(selector &&other) = delete;
	virtual ~selector(){}
/*
	selector& operator=(const selector &other);
	selector& operator=(selector &&other);
*/

	std::unique_ptr<gui::OutputForm> showReport(int position, int size){
		std::unique_ptr<gui::OutputForm> report(new gui::OutputForm());
//		int count = 0;
		pqxx::work statement{*getConnection()};

		std::string sqlExec ( select_sql.str() );

		bool isRange = calculateDateRange(position, size);

		pqxx::result ret_data { statement.exec(isRange?sqlExec.append(range.str()):sqlExec) };

		ssize_t column_size = ret_data.columns();

		for (ssize_t column_index = 0; column_index < column_size; column_index++){
			report->add_column(ret_data.column_name(column_index));
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
/*
			if ((count++) >= size) {
				break;
			}
			*/
		}

		return std::move(report);
	}
};

} /* namespace db */

#endif /* DB_SELECTOR_H_ */
