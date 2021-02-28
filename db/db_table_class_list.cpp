/*
 * db_table_class_list.cpp
 *
 *  Created on: Dec 16, 2020
 *      Author: Agapov Aleksey
 */
#include "db_table_class_list.h"
#include "selector.h"


namespace db {

std::optional<std::string> nl(std::string s) {
    if (s != "") { return s; }
    else { return {}; }
}



std::unique_ptr<selector> TabelControl::getTablesShow(std::vector<int>& tabs){
	vector<std::shared_ptr<InterfaceTable> > new_table_list;
	std::for_each(std::begin(tabs), std::end(tabs), [&](int tab_index){
		new_table_list.push_back(getTableInfo(tab_index, false));
	});
	return std::move(std::make_unique<selector>(new_table_list));
}

std::unique_ptr<selector> TabelControl::getTablesShow(std::vector<std::pair<std::string,std::string> >& tabs){
	vector<std::shared_ptr<InterfaceTable> > new_table_list;
	std::for_each(std::begin(tabs), std::end(tabs), [&](std::pair<std::string,std::string> tab_info){
		new_table_list.push_back(getTableInfo(tab_info.first, tab_info.second, false));
	});
	return std::move(std::make_unique<selector>(new_table_list));
}




column_info ColumnInfo::getColumnInfo() {
//	typedef std::tuple<       int,       std::string, int, std::string,     std::string,          std::string, std::string> column_info;
	return {column_index, label, size, col_type_label, col_constraint_label, fk_schema, fk_table, fk_column};
}


std::unique_ptr<gui::OutputForm> reportTablesList(std::shared_ptr<db_tables_info> tables_list){
	std::unique_ptr<gui::OutputForm> report(new gui::OutputForm());
	report->add_column("number",gui::OutputForm::ColumnType::NUM).add_column("schema").add_column("table");

	std::for_each(std::begin(*tables_list), std::end(*tables_list), [&](DbObject &table){
		report->add_row({std::to_string(table.index), table.schema, table.table});
	});

	return std::move(report);
}



std::unique_ptr<gui::OutputForm> reportTablesInfo(const db::HeaderInfo& tables_info){
	std::unique_ptr<gui::OutputForm> report(new gui::OutputForm());
	report->add_column("number", gui::OutputForm::ColumnType::NUM).add_column("label").add_column("type").add_column("size", gui::OutputForm::ColumnType::NUM).add_column("constraint").add_column("fk_schema").add_column("fk_table").add_column("fk_column");

	int column_index, size;
	std::string label, col_type_label, col_constraint_label, fk_schema, fk_table, fk_column;
	for (db::ColumnInfo column:tables_info) {
		std::tie(column_index, label, size, col_type_label, col_constraint_label, fk_schema, fk_table, fk_column) = column.getColumnInfo();
		report->add_row({std::to_string(column_index), label, col_type_label, std::to_string(size), col_constraint_label, fk_schema, fk_table, fk_column});
	}

return std::move(report);
}






const std::string& ColumnInfo::getFkColumn() const {
	return fk_column;
}

const std::string& ColumnInfo::getFkSchema() const {
	return fk_schema;
}

const std::string& ColumnInfo::getFkTable() const {
	return fk_table;
}

}
