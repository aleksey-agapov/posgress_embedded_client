/*
 * OutputForm.h
 *
 *  Created on: Dec 22, 2020
 *      Author: Agapov Aleksey
 */

#ifndef GUI_OUTPUTFORM_H_
#define GUI_OUTPUTFORM_H_

#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <sstream>
#include "../control/Log.h"


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
class OutputForm : protected control::Log {
public:
	enum class ColumnType { NUM=0, BOOL, STRING, DATE };
private:
	int column_size;
	std::vector<std::string> header_label;
	std::vector<std::string::size_type> columns_size;
	std::vector<std::vector<std::string> > data;
	std::vector<ColumnType> type_list;
	std::string title;

	void calcSizeColumn(std::vector<std::string> new_row);

public:
	OutputForm(): control::Log("OutputForm"), column_size(0){	}
	virtual ~OutputForm() = default;

	OutputForm& add_column(const char* new_column, ColumnType type = ColumnType::STRING);
	bool add_row(std::vector<std::string> new_row);
	std::string& getValue(int row_id,int column_id);

	size_t getRowSize() const                            { return data.size(); }
	std::vector<std::string>& getHeaderLabel()           { return header_label; }
	std::vector<std::string::size_type>& getColumnSize() { return columns_size; }
	std::vector<ColumnType>& getColumnType()             { return type_list; }
	std::string getTitle() const                         { return title; }
	void setTitle(std::string title)                     { this->title = title; }

	std::vector<std::string>& operator[](int row_id) { return data[row_id]; }
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



class printer : protected control::Log  {
	std::ostream& output;
	printerTableStyle style;
	std::string::size_type table_size;
	std::unique_ptr<OutputForm> tableForm;
	bool isShowNum;
	std::string::size_type row_count;

	inline static const char * NumFieldTitle = "№ row";
	inline static std::string::size_type NumFieldSize = strlen(NumFieldTitle);

	void calcSize();
	void showUpLine();
	void showDownLine();
	void showTitle();
	void showHeader(std::vector<std::string>& header);
	void showRow(std::vector<std::string>& row, std::vector<OutputForm::ColumnType>& columnType);
public:
	printer() :
		printer(std::cout, false) {}
	printer(std::ostream& out_stream , bool showNum = false) :
		printer(out_stream,  printerTableStyle::getDefault(), showNum) {}
	printer(std::ostream& out_stream, printerTableStyle new_style, bool showNum = false) :
		control::Log("printer"),
		output(out_stream),
		style(new_style),
		table_size(0),
		isShowNum(showNum),
		row_count(0)
	{}

	std::unique_ptr<OutputForm> showForm(std::unique_ptr<OutputForm> result, int new_row_count = 0) ;
};

} /* namespace gui */

#endif /* GUI_OUTPUTFORM_H_ */
