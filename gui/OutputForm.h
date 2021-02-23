/*
 * OutputForm.h
 *
 *  Created on: Dec 22, 2020
 *      Author: root
 */

#ifndef GUI_OUTPUTFORM_H_
#define GUI_OUTPUTFORM_H_
#include <algorithm>
#include <string>
#include <array>
#include <vector>
#include <exception>
#include <string.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <iomanip>

namespace gui {


class guiException : public std::exception {
	std::string msg;
public:
	guiException(std::string new_msg){
		msg = new_msg;
	}

   const char * what () const noexcept {
      return msg.c_str();
   }
};


//template <int size = 10>
class OutputForm  {

public:

	enum class ColumnType { NUM=0, BOOL, STRING, DATE };

	OutputForm(): column_size(0){	}

	virtual ~OutputForm() = default;


	OutputForm& add_column(const char* new_column, ColumnType type = ColumnType::STRING){
			header_label.push_back(std::string(new_column));
			columns_size.push_back(strlen(new_column));
			type_list.push_back(type);
			column_size++;
		return *this;
	}


	bool add_row(std::vector<std::string> new_row) {
		try {
			data.push_back(new_row);
			calcSizeColumn(new_row);
		} catch (...) {return false;}
		return true;
	}

	size_t getRowSize() const {
		return data.size();
	}

	std::string& getValue(int row_id,int column_id)        // индексатор
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

	std::vector<std::string>& getHeaderLabel() {return header_label;}
	std::vector<std::string::size_type>& getColumnSize() {return columns_size;}
	std::vector<ColumnType>& getColumnType() {return type_list;}

	std::string getTitle() {
		return title;
	}

	void setTitle(std::string title) {
		this->title = title;
	}

	std::vector<std::string>& operator[](int row_id)        // индексатор
    {
		return data[row_id];
    }
private:
	int column_size;
	std::vector<std::string> header_label;
	std::vector<std::string::size_type> columns_size;
	std::vector<std::vector<std::string> > data;
	std::vector<ColumnType> type_list;

	std::string title;




	void calcSizeColumn(std::vector<std::string> new_row) {
		for (int count=0;count<column_size;count++) {
			if (new_row[count].size() > columns_size[count]) {
				 columns_size[count] = new_row[count].size();
			}
		}
	}

};


struct printerTableStyle{
	char top_left;
	char top_column;
	char top_sep;
	char top_right;

	char center_left;
	char center_column;
	char center_sep;
	char center_right;

	char down_left;
	char down_column;
	char down_sep;
	char down_right;

	char row_line_empty;
	unsigned char column_line_empty;

	static printerTableStyle getDefault() {
		return {
			'+',	// top_left
			'+',	// top_column
			'-',	// top_sep
			'+',	// top_right

			'|',	// center_left;
			'|',	// center_column;
			' ',	// center_sep;
			'|',	// center_right;

			'+',	// down_left;
			'+',	// down_column;
			'-',	// down_sep;
			'+',	// down_right;

			' ',	// row_line_empty;
			1		// column_line_empty;
		};
	}
};



class printer {
	std::ostream& output;
	printerTableStyle style;
	std::string::size_type table_size;
	std::unique_ptr<OutputForm> tableForm;

	std::string
	centered( std::string const& original, int targetSize, char sep )
	{
	    int padding = targetSize - static_cast<int>( original.size() );
	    return padding > 0
	        ? std::string( padding / 2, sep )
	            + original
	            + std::string( targetSize - (padding / 2), sep )
	        : original;
	}


	void calcSize(){
		table_size = 0;
		std::vector<std::string::size_type>& columnSize =tableForm->getColumnSize() ;
		std::for_each(std::begin(columnSize), std::end(columnSize), [&](std::string::size_type col_size){table_size+=col_size+style.column_line_empty*2;});
		table_size+=2;
		if (!columnSize.empty()) {
			table_size+=style.column_line_empty*(columnSize.size()-1);
		}
	}


	void showUpLine(){
		std::vector<std::string::size_type>& columnSize =tableForm->getColumnSize() ;
		int columns_insert = columnSize.size()-1;
		int column_count = 0;
		output << style.top_left;
		std::for_each(std::begin(columnSize), std::end(columnSize),
				[&](std::string::size_type col_size){
					int full_col_size=col_size+style.column_line_empty*2;
					output << std::setfill(style.top_sep) << std::setw(full_col_size) << style.top_sep;
					if (column_count < columns_insert) {
						output << style.top_column;
					}
					column_count++;
				}
		);
		output << style.top_right << std::endl;
	}

	void showDownLine() {
		std::vector<std::string::size_type>& columnSize =tableForm->getColumnSize() ;
		int columns_insert = columnSize.size()-1;
		int column_count = 0;
		output << style.down_left;
		std::for_each(std::begin(columnSize), std::end(columnSize),
				[&](std::string::size_type col_size){
					int full_col_size=col_size+style.column_line_empty*2;
					output << std::setfill(style.down_sep) << std::setw(full_col_size) << style.down_sep;
					if (column_count < columns_insert) {
						output << style.down_column;
					}
					column_count++;
				}
		);
		output << style.top_right << std::endl;
	}

	void showTitle() {
		if (!tableForm->getTitle().empty()) {
			output << centered(tableForm->getTitle(),table_size,style.row_line_empty) << std::endl;
		}
	}



	void showHeader(std::vector<std::string>& header) {
		std::vector<std::string::size_type>& columnSize =tableForm->getColumnSize() ;
		int columns_insert = columnSize.size()-1;
		int column_count = 0;
		output << style.center_left;
		std::for_each(std::begin(columnSize), std::end(columnSize),
				[&](std::string::size_type col_size){
					if (style.column_line_empty > 0) output << std::setfill(style.center_sep) << std::setw(style.column_line_empty) << style.center_sep;
					output << std::left << std::setfill(style.center_sep) << std::setw(col_size) << header[column_count];
					if (style.column_line_empty > 0) output << std::setfill(style.center_sep) << std::setw(style.column_line_empty) << style.center_sep;
					if (column_count < columns_insert) {
						output << style.center_column;
					}
					column_count++;
				}
		);
		output << style.center_right << std::endl;
	}


	void showRow(std::vector<std::string>& row, std::vector<OutputForm::ColumnType>& columnType) {
		std::vector<std::string::size_type>& columnSize =tableForm->getColumnSize() ;
		int columns_insert = columnSize.size()-1;
		int column_count = 0;
		output << style.center_left;
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

public:
	printer() :
		printer(std::cout) {}
	printer(std::ostream& out_stream ) :
		printer(out_stream,  printerTableStyle::getDefault()) {}
	printer(std::ostream& out_stream, printerTableStyle new_style ) :
		output(out_stream),
		style(new_style),
		table_size(0){}

	std::unique_ptr<OutputForm> showForm(std::unique_ptr<OutputForm> result) {
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
};

} /* namespace gui */

#endif /* GUI_OUTPUTFORM_H_ */
