/*
 * db_table_class_list.h
 *
 *  Created on: Nov 14, 2020
 *      Author: root
 */

#ifndef DB_DB_TABLE_CLASS_LIST_H_
#define DB_DB_TABLE_CLASS_LIST_H_
#include <pqxx/pqxx>
#include <string>
#include <cassert>
#include <vector>
#include <any>
#include <memory>
#include <iostream>
#include <optional>
#include <algorithm>
#include "db_info_const.h"
#include "../gui/OutputForm.h"
#include "../config/AppConfigControl.h"










using namespace std;

namespace db {

using ColumnType =  gui::OutputForm::ColumnType;

class  Log;
struct DbObject;
class  InterfaceTabelControl;
class  Table;
class  RowInfo;
class  ColumnInfo;
class  selector;

typedef std::shared_ptr<db::Table> FkTable;
typedef std::vector<DbObject> db_tables_info; // @suppress("Invalid template argument")
typedef struct DbObject db_table_info;
typedef std::tuple<int, std::string, int, std::string, std::string, std::string, std::string, std::string> column_info;

typedef std::vector<RowInfo> RowData;
typedef std::vector<ColumnInfo> HeaderInfo;


extern std::optional<std::string> nl(std::string s);
extern std::unique_ptr<gui::OutputForm> reportTablesList(std::shared_ptr<db_tables_info>    tables_info);
extern std::unique_ptr<gui::OutputForm> reportTablesInfo(const db::HeaderInfo& tables_info);






inline static ColumnType getType(pqxx::oid sql_type) {
	ColumnType type_data = ColumnType::STRING;
      switch (sql_type) {
         case 23: type_data = ColumnType::NUM; break; // INTEGER
         case 1700: case 701: type_data = ColumnType::NUM; break; // double
         case 8: case 1042: case 1043: type_data = ColumnType::STRING; break; // c.type="std::string";
         case 16: type_data = ColumnType::NUM;  break;   // "bool";
         case 21: type_data = ColumnType::NUM;  break;  // "short"
         case 20: type_data = ColumnType::NUM;  break;  // "long long"
         // TODO these types are currently only returned as strings, should pick proper C++ types
         case 17 /*bytea*/: case 1082 /*date*/: case 1083 /*time*/: case 1114 /*timestamp*/:
         case 1186 /*interval*/: case 704 /*interval*/: case 114 /*JSON*/: case 705 /*Unknown*/:
        	 type_data = ColumnType::STRING; break;
         default:
        	 type_data = ColumnType::STRING;
      }
	return type_data;
}




class InterfaceTable {
public:
	virtual const std::string getName() const = 0;
	virtual const std::string getSchema() const = 0;
	virtual const HeaderInfo& getHeaderInfo() const = 0;
	virtual ~InterfaceTable(){}
};



/*
 * class Log
 */
class Log {
	std::string Tag;

	public:
//		Log() = delete;
		Log (std::string tag) : Tag(tag){}
		virtual ~Log(){}

		void setLogTag(std::string tag) {Tag = {tag};}
		void msg(const char * log_msg) const {std::cout << Tag << " ==> " << log_msg << std::endl; }

};




/*
 * class PgConnection
 */
class PgConnection : public config::IConfigCallBack {   // , public std::enable_shared_from_this<config::IConfigCallBack>
private:
	Log log;
	std::shared_ptr<config::DbAppConfig> db_config;
	inline static std::shared_ptr<pqxx::connection> db_connect;

	bool isReady() {
		if (db_connect) {
			if (!db_connect->is_open()) {
				try {
					db_connect->disconnect();
				} catch (...) {}
				db_connect = nullptr;
			}
		}
		return (db_connect)?true:false;
	}


public:
	PgConnection(): log("PgConnection"){
		if (!isReady())  throw invalid_argument("No active connection.");
	}

	PgConnection(std::string connect_parms): log("PgConnection"){
//		isReady();
		try {
			db_connect = std::make_shared<pqxx::connection>(connect_parms);
		}
		catch (...){}
//		if (!isReady())  throw invalid_argument("The argument not correct.");
	}

	void setConfigControl(std::shared_ptr<config::DbAppConfig> db_config) {
		this->db_config = db_config;
		db_config->RegisterCallback(db_config->getModuleName(), std::shared_ptr<config::IConfigCallBack>(this));
	}


	void Update() {
		isReady();
		std::cout << "open database:" << db_config->getConfigLine() << std::endl;
		db_connect = std::make_shared<pqxx::connection>(db_config->getConfigLine());

		if (db_connect->is_open()) {
			std::cout << "Opened database successfully: " << db_connect->dbname() << std::endl;
			std::cout << "              server version: " << db_connect->server_version() << std::endl;
			std::cout << "            protocol version: " << db_connect->protocol_version() << std::endl;
		} else {
			std::cerr << "Can't open database:" << db_config->getConfigLine() << std::endl;
		}
	}


	static std::shared_ptr<pqxx::connection> getConnection() {
		return db_connect;
	}

	virtual ~PgConnection(){}
};

#define getRowValue(columnName, row_table, rowValue, ret_obj, retType) auto rowValue = row_table.at(columnName); \
if (!rowValue.is_null()) { \
	ret_obj = rowValue.as<retType>(); \
}



 /*
  * class column_info
  */
class ColumnInfo : private TypeInfo, private Log, private PgConnection {
public:
	 ColumnInfo () = delete;
	 ColumnInfo(std::string new_label, int new_size, Type new_type): Log("ColumnInfo"),
		 label(new_label),
		 size(new_size),
		 data_type(new_type)
	 {
		 SetDefault();
	 }

	 ColumnInfo(pqxx::row & row_table) : Log("ColumnInfo") {
		SetDefault();
		getRowValue(COLUMN,row_table, row0,label, std::string)		//		label        = row_table.at(COLUMN).as<std::string>();
		getRowValue(COLUMN_NUMBER,row_table,row1,column_index, int)	//		column_index = row_table.at(COLUMN_NUMBER).as<int>();
		getRowValue(COLUMN_SIZE,row_table,row2,size, int)	//		size = row_table.at(COLUMN_SIZE).as<int>();
		getRowValue(DATA_TYPE1,row_table,row3,col_type_label, std::string)		//		col_type_label        = row_table.at(DATA_TYPE1).as<std::string>();
		data_type = getTypeNeme(col_type_label);

	 }

	void setConstraintInfo(InterfaceTabelControl &control, pqxx::row & row_table) {
		getRowValue(CONSTRAINT,row_table,row0,col_constraint_label, std::string)
		if (col_constraint_label.compare( CONST_FOREIGN_KEY ) == 0) {
			getRowValue(FK_SCHEMA,row_table,row1,fk_schema, std::string)
			getRowValue(FK_TABLE,row_table,row2,fk_table, std::string)
			getRowValue(FK_COLUMN,row_table,row3,fk_column, std::string)
			setIsFk(true);
//			FK = control.getTableInfo(fk_schema, fk_table, false);
		} else if (col_constraint_label.compare( CONST_PRIMARY_KEY ) == 0) {
			setIsPk(true);
		}
	}

//	const std::shared_ptr<InterfaceTable> getFk() const {return FK;};
	bool isFk() const { return isFK; }; // getBitBoolVal(isFK);
	bool isPk() const { return isPK; }; // getBitBoolVal(isPK);
	bool isUnique() const { return isUNIQUE; }; //getBitBoolVal(isUNIQUE);
	const std::string& getLabel() const{ return label; } ;
	int getSize() const{return size;}
	Type getType() const{return data_type;}

	void setIsFk(bool Fk) {isFK = Fk; }; //setBitBoolVal(isFK,Fk);
	void setIsPk(bool Pk) {isPK = Pk; }; //setBitBoolVal(isPK,Pk);
	void setIsUnique(bool Unique) {isUNIQUE = Unique; }; //setBitBoolVal(isPK,Unique);
	void setLabel(std::string &new_label) { label = new_label; }
	void setSize(int new_size) {size=new_size;}
	void setType(Type new_type){data_type=new_type;}
	column_info getColumnInfo();
	const std::string& getFkColumn() const;
	const std::string& getFkSchema() const;
	const std::string& getFkTable() const;

//	 std::make_tuple

private:
	std::string label;
	int column_index;
	int size;
	Type data_type;
	std::string col_type_label;
	std::string col_constraint_label;
	bool isPK;
	bool isUNIQUE;
	bool isFK;
//	std::shared_ptr<InterfaceTable> FK;
	std::string fk_schema;
	std::string fk_table;
	std::string fk_column;

	void SetDefault() {
		column_index = -1;
		data_type = Type::OTHER;
		size = 0;
		 setIsFk(false);
		 setIsPk(false);
		 setIsUnique(false);
//		 FK = nullptr;
	}

};

/*
 * class row_info
 */
class RowInfo {
	HeaderInfo& column_info;
	std::any row_data;
public :
	RowInfo(HeaderInfo& new_column_info, std::any new_data = nullptr): column_info(new_column_info), row_data(new_data){}

	virtual ~RowInfo(void){}

	const std::any& getRowData() const {
		return row_data;
	}

	void setRowData(const std::any &rowData) {
		row_data = rowData;
	}
};


/*
 * class Table
 */
class Table : virtual public InterfaceTable, private db::Log, private db::PgConnection{
	std::string name;
	std::string schema;
	HeaderInfo header_info;
	RowData row_data;

		bool updateTableInfo(InterfaceTabelControl &control) {
			try {
				pqxx::work statement { *getConnection() };
				header_info.clear();
				char sql_buffer[4096];
				sprintf(sql_buffer, LIST_TABLES_COLUMN, schema.c_str(), name.c_str());
				msg(sql_buffer);
				pqxx::result ret_column_data { statement.exec(sql_buffer) };

				for (pqxx::row row : ret_column_data) {
					ColumnInfo column(row);
					header_info.push_back(column);
				}


				sprintf(sql_buffer, LIST_TABLES_COLUMN_REF, schema.c_str(), name.c_str());
				msg(sql_buffer);
				pqxx::result ret_ref_data { statement.exec(sql_buffer) };
				for (pqxx::row row : ret_ref_data) {
					std::for_each(std::begin(header_info), std::end(header_info), [&](ColumnInfo &column){
						std::string curent_col = row.at(COLUMN).as<std::string>();
						if (column.getLabel().compare(curent_col) == 0) {
							column.setConstraintInfo(control, row);
						}
					});
				}

				return true;
			} catch (const std::exception &e) {
				std::cerr << e.what() << std::endl;
				header_info.clear();
			}
			return false;
		}


		void setTableName(std::string& new_name) {
			name = new_name;
			updateLogTag();
		}
		void setSchemName(std::string& new_schema) {
			schema = new_schema;
			updateLogTag();
		}

		void updateLogTag(){
			setLogTag(std::string(typeid(this).name()).append("_").append(name).append("_").append(schema));
		}


	public :
		Table () = delete;

		Table(InterfaceTabelControl &control, const char * new_name, const char * new_schema) : Log("Table"){	//  :Log{std::ref(string("Table")}
			assert(new_name && "initializer new name is empty!");
			name = new_name;
			if (new_schema) {schema = new_schema;}
			updateLogTag();
			updateTableInfo(control);
			msg("constructor parms");
		}

		virtual ~Table(){}


	const std::string getName() const {
		return name;
	}

	const std::string getSchema() const {
		return schema;
	}

	const HeaderInfo& getHeaderInfo() const {
		return header_info;
	}
};




struct DbObject {
	int index;
	std::string schema;
	std::string table;
	std::shared_ptr<db::Table> prtTable;

	std::string getFullName() {
		return schema+"."+table;
	}
};


class InterfaceTabelControl {
public :
	virtual std::shared_ptr<InterfaceTable> getTableInfo(int number, bool isUpdate = false)  = 0;
	virtual std::shared_ptr<InterfaceTable> getTableInfo(std::string sel_schema, std::string sel_table, bool isUpdate = false) = 0;
	virtual ~InterfaceTabelControl() {}
};


class TabelControl : virtual public InterfaceTabelControl, private Log, private PgConnection{
	private:
		std::shared_ptr<db_tables_info>    tables_info; // @suppress("Invalid template argument")
		std::string schema;

		void RefreshTableList() {
				int count = 0;
				pqxx::work statement{*getConnection()};
				tables_info->clear();
				pqxx::result ret_data { statement.exec(LIST_TABLES) };
				for (auto row: ret_data) {
					DbObject table_info;
					table_info.index    = count;
					table_info.schema   = row[0].as<std::string>();
					table_info.table    = row[1].as<std::string>();
					table_info.prtTable = nullptr;
					tables_info->push_back(table_info);
					count++;
				}
		}

		std::shared_ptr<db_tables_info> getTableInfo(bool isUpdate) {
			if (isUpdate) {
				RefreshTableList();
			}
			return tables_info;
		}


	public:
		TabelControl() : Log("TabelControl") {
			tables_info = std::make_shared<db_tables_info>();
			RefreshTableList();
		}

		virtual ~TabelControl(){}


		void setSchema(std::string new_schema){
			this->schema = new_schema;
		}

		std::shared_ptr<db_tables_info> getTablesList() {
			return tables_info;
		}

		bool isTableListEmpty() {
			if(tables_info) {
				return tables_info->empty();
			}
			return false;
		}

		std::shared_ptr<InterfaceTable> getTableInfo(int number, bool isUpdate = false) {
			std::shared_ptr<Table> table;
			for (db::db_table_info elem : *getTableInfo(isUpdate)) {
				if (elem.index == number) {
					if ((elem.prtTable)&& (!isUpdate)) {table = elem.prtTable;}
					else {
						table = std::make_shared<Table>(*this, elem.table.c_str(), elem.schema.c_str());
						elem.prtTable = table;
					}
				}
			}
			return static_cast<std::shared_ptr<InterfaceTable> > (table);
		}


		std::shared_ptr<InterfaceTable> getTableInfo(std::string sel_table, bool isUpdate = false) {
			return getTableInfo(schema, sel_table, isUpdate);
		}


		std::shared_ptr<InterfaceTable> getTableInfo(std::string sel_schema, std::string sel_table, bool isUpdate = false) {
			std::shared_ptr<Table> table;
			for (db::db_table_info elem : *getTableInfo(isUpdate)) {
				if ((elem.schema.compare(sel_schema) == 0) && (elem.table.compare(sel_table) == 0)) {
					if ((elem.prtTable)&& (!isUpdate)) {table = elem.prtTable;}
					else {
						table = std::make_shared<Table>(*this, elem.table.c_str(), elem.schema.c_str());
						elem.prtTable = table;
					}
				}
			}
			return static_cast<std::shared_ptr<InterfaceTable> > (table);
		}


		std::unique_ptr<selector> getTablesShow(std::vector<int>& tabs);

		std::unique_ptr<selector> getTablesShow(std::vector<std::pair<std::string, std::string> >& tabs);

};

}



#endif /* DB_DB_TABLE_CLASS_LIST_H_ */
