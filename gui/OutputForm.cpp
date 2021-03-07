/*
 * OutputForm.cpp
 *
 *  Created on: Dec 22, 2020
 *      Author: Agapov Aleksey
 */
#include <algorithm>
#include <iomanip>
#include <string.h>
#include <exception>
#include "OutputForm.h"
#include "../utils/apputils.h"

namespace gui {


OutputForm& OutputForm::add_column(const char* new_column, ColumnType type){
		header_label.push_back(std::string(new_column));
		columns_size.push_back(strlen(new_column));
		type_list.push_back(type);
		column_size++;
	return *this;
}


bool OutputForm::add_row(std::vector<std::string> new_row) {
	try {
		data.push_back(new_row);
		calcSizeColumn(new_row);
	} catch (...) {return false;}
	return true;
}


std::string& OutputForm::getValue(int row_id,int column_id)        // индексатор
{
	if (row_id >= static_cast<int>(data.size())) {
		char exception_msg[1024];
		sprintf(exception_msg,"Row_id:%d is more then row_size:%lu\n" , row_id, data.size());
		throw new guiException(exception_msg);
	}
	if (column_id >= column_size) {
		char exception_msg[1024];
		sprintf(exception_msg,"Column_id:%d is more then column_size:%d\n" , column_id, column_size);
		throw new guiException(exception_msg);
	}
	return data[row_id][column_id];
}

void OutputForm::calcSizeColumn(std::vector<std::string> new_row) {
	for (int count=0;count<column_size;count++) {
		if (new_row[count].size() > columns_size[count]) {
			 columns_size[count] = new_row[count].size();
		}
	}
}

// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void printer::calcSize(){
	table_size = 0;
	std::vector<std::string::size_type>& columnSize = tableForm->getColumnSize() ;
	auto calk_column_length = [&](std::string::size_type col_size){table_size += col_size+style.column_line_empty*2;};

	if (isShowNum) {calk_column_length(NumFieldSize);}

	std::for_each(std::begin(columnSize), std::end(columnSize), calk_column_length);
	table_size += 2;
	if (!columnSize.empty()) {
		table_size += style.column_line_empty*(columnSize.size()-1);
	}
}

void printer::showUpLine(){
	std::vector<std::string::size_type>& columnSize = tableForm->getColumnSize() ;
	int columns_insert = columnSize.size()-1;
	int column_count = 0;

	auto build_upper_table_line = [&](std::string::size_type col_size){
		int full_col_size=col_size+style.column_line_empty*2;
		output << std::setfill(style.top_sep) << std::setw(full_col_size) << style.top_sep;
		if (column_count < columns_insert) {
			output << style.top_column;
		}
		column_count++;
	};

	output << style.top_left;
	if (isShowNum) {
		int full_col_size=NumFieldSize+style.column_line_empty*2;
		output << std::setfill(style.top_sep) << std::setw(full_col_size) << style.top_sep << style.top_column;
	}
	std::for_each(std::begin(columnSize), std::end(columnSize), build_upper_table_line);
	output << style.top_right << std::endl;
}

void printer::showDownLine() {
	std::vector<std::string::size_type>& columnSize = tableForm->getColumnSize() ;
	int columns_insert = columnSize.size()-1;
	int column_count = 0;
	auto build_down_table_line = [&](std::string::size_type col_size){
		int full_col_size = col_size+style.column_line_empty*2;
		output << std::setfill(style.down_sep) << std::setw(full_col_size) << style.down_sep;
		if (column_count < columns_insert) {
			output << style.down_column;
		}
		column_count++;
	};

	output << style.down_left;
	if (isShowNum) {
		int full_col_size = NumFieldSize+style.column_line_empty*2;
		output << std::setfill(style.down_sep) << std::setw(full_col_size) << style.down_sep << style.down_column;
	}

	std::for_each(std::begin(columnSize), std::end(columnSize),build_down_table_line);
	output << style.top_right << std::endl;
}

void printer::showTitle() {
	if (!tableForm->getTitle().empty()) {
		output << utils::centered(tableForm->getTitle(),table_size,style.row_line_empty) << std::endl;
	}
}



void printer::showHeader(std::vector<std::string>& header) {
	std::vector<std::string::size_type>& columnSize = tableForm->getColumnSize() ;
	int columns_insert = columnSize.size()-1;
	int column_count = 0;
	auto build_table_header = [&](std::string::size_type col_size){
		if (style.column_line_empty > 0) output << std::setfill(style.center_sep) << std::setw(style.column_line_empty) << style.center_sep;
		output << std::left << std::setfill(style.center_sep) << std::setw(col_size) << header[column_count];
		if (style.column_line_empty > 0) output << std::setfill(style.center_sep) << std::setw(style.column_line_empty) << style.center_sep;
		if (column_count < columns_insert) {
			output << style.center_column;
		}
		column_count++;
	};

	output << style.center_left;
	if (isShowNum) {
		int full_col_size = NumFieldSize+style.column_line_empty*2;
		if (style.column_line_empty > 0) output << std::setfill(style.center_sep) << std::setw(style.column_line_empty) << style.center_sep;
		output << std::left << std::setfill(style.center_sep) << std::setw(full_col_size) << NumFieldTitle;
		if (style.column_line_empty > 0) output << std::setfill(style.center_sep) << std::setw(style.column_line_empty) << style.center_sep;
		output << style.center_column;
	}
	std::for_each(std::begin(columnSize), std::end(columnSize),build_table_header);
	output << style.center_right << std::endl;
}


void printer::showRow(std::vector<std::string>& row, std::vector<OutputForm::ColumnType>& columnType) {
	std::vector<std::string::size_type>& columnSize =tableForm->getColumnSize() ;
	int columns_insert = columnSize.size()-1;
	int column_count = 0;
	output << style.center_left;

	if (isShowNum) {
		if (style.column_line_empty > 0) {output << std::setfill(style.center_sep) << std::setw(style.column_line_empty) << style.center_sep;}
		output << std::right << std::setfill(style.center_sep) << std::setw(NumFieldSize) << std::to_string(row_count);
		if (style.column_line_empty > 0) {output << std::setfill(style.center_sep) << std::setw(style.column_line_empty) << style.center_sep;}
		output << style.center_column;
		row_count++;
	}


	std::for_each(std::begin(columnSize), std::end(columnSize),
			[&](std::string::size_type col_size){
				if (style.column_line_empty > 0) output << std::setfill(style.center_sep) << std::setw(style.column_line_empty) << style.center_sep;

				switch (columnType[column_count] ) {
					case OutputForm::ColumnType::NUM:
					case OutputForm::ColumnType::DATE:
					case OutputForm::ColumnType::BOOL:
						output << std::right;
						break;
					default:
						output << std::left;
				}

				output << std::setfill(style.center_sep) << std::setw(col_size) << row[column_count];
				if (style.column_line_empty > 0) output << std::setfill(style.center_sep) << std::setw(style.column_line_empty) << style.center_sep;
				if (column_count < columns_insert) {
					output << style.center_column;
				}
				column_count++;
			}
	);
	output << style.center_right << std::endl;
}


std::unique_ptr<OutputForm> printer::showForm(std::unique_ptr<OutputForm> result, int new_row_count) {
	row_count = new_row_count;
	output.clear();
	output << std::endl;
	tableForm = std::move(result);
	if (tableForm) {
		calcSize();
		showTitle();
		showUpLine();
		showHeader(tableForm->getHeaderLabel());
		showUpLine();
		for (int index = 0; index < static_cast<int>(tableForm->getRowSize()); index++) {
			showRow((*tableForm)[index], tableForm->getColumnType());
		}
		showDownLine();
	}
	return std::move(tableForm);
}

} /* namespace gui */

