/*
 * selector.cpp
 *
 *  Created on: Dec 28, 2020
 *      Author: Agapov Aleksey
 */

#include "selector.h"

namespace db {

selector::selector(vector<std::shared_ptr<InterfaceTable> > new_table_list): PgConnection("selector"), table_list(new_table_list){
	if(!table_list.empty()){
		int count = 0;
		std::for_each(std::begin(table_list), std::end(table_list), [&](std::shared_ptr<InterfaceTable>& table) {
			if (count) {
				from << ",";
			}
			calc_column(count);
			from << table->getSchema() << "." << table->getName() << " T"  << count++;
		}
		);

		if ((!from.str().empty()) && (!fields.str().empty())) {
			select_sql << "SELECT "<< fields.str() << " FROM "<< from.str();
			if (!where.str().empty()) {
				select_sql << " WHERE "<< where.str();
			}
			msg(select_sql.str());
		}
	}
}


int selector::isFoundSelectTable(int select_count, const std::string & schema, const std::string & table) {
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


void selector::calc_column(int count) {

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

bool selector::calculateDateRange(int position, int size){
	range.clear();
	if (position) {
		range << " OFFSET " << position;
	}

	if (size) {
		range << " LIMIT " << size << " ";
	}
	return !range.str().empty();
}

std::unique_ptr<gui::OutputForm> selector::showReport(int position, int size, const std::vector<ssize_t> & show_columns_num){
	std::unique_ptr<gui::OutputForm> report(new gui::OutputForm());
	if (isReady()) {
		std::string title;
		pqxx::work statement{*getConnection()};
		std::string sqlExec ( select_sql.str() );

		bool isRange = calculateDateRange(position, size);

		pqxx::result ret_data { statement.exec(isRange?sqlExec.append(range.str()):sqlExec) };

		ssize_t column_size = ret_data.columns();

		title.append("Show records:").append(std::to_string(ret_data.size())).append(" Column size:").append(std::to_string(column_size));

		std::vector<ssize_t> list_column_index;
		if (!show_columns_num.empty()) {
			title.append(" Columns show:");
			for (ssize_t index_column: show_columns_num) {
				if ((index_column > 0) && (index_column <= column_size)) {
					msg( "showReport add column index:", index_column, " column_size: ", column_size);
					list_column_index.push_back(index_column-1);
					title.append(" ").append(std::to_string(index_column));
				}
			}
		}

		auto add_header_to_report = [&ret_data, &report](ssize_t column_index ) {
			ColumnType columnType = getType(ret_data.column_type(static_cast<int>(column_index)));
			report->add_column(ret_data.column_name(column_index), columnType);
		};

		if (list_column_index.empty()) {
			for (ssize_t column_index = 0; column_index < column_size; column_index++){
				add_header_to_report(column_index);
			}
		} else {
			std::for_each(std::cbegin(list_column_index), std::cend(list_column_index), add_header_to_report);
		}

		for (auto row: ret_data) {
			auto new_row = vector<std::string> {};
			if (list_column_index.empty()) {
				for (ssize_t column_index = 0; column_index < column_size; column_index++){
					new_row.push_back(
						row[static_cast<int>(column_index)].is_null()?
						"":
						row[static_cast<int>(column_index)].as<std::string>()
					);
				}
			} else {
				for (auto column_index: list_column_index) {
					new_row.push_back(
						row[static_cast<int>(column_index)].is_null()?
						"":
						row[static_cast<int>(column_index)].as<std::string>()
					);
				}
			}
			report->add_row(new_row);
		}

		report->setTitle(title);
	} else {
		msg("Error: Connection lost.");
	}
	return std::move(report);
}


} /* namespace db */
