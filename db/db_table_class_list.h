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
#include <map>
#include "db_info_const.h"









using namespace std;

namespace db {

class Table ;
class ColumnInfo;
class RowInfo;

typedef std::vector<ColumnInfo> HeaderInfo;
typedef std::vector<RowInfo> RowData;

typedef std::multimap<std::string, std::string> db_tables_info; // @suppress("Invalid template argument")
typedef pair <std::string, std::string> db_table_info;
/*
 * class Log
 */
class Log {
	std::string& Tag;
	protected:
		void msg(const char * log_msg) const {std::cout << Tag << " " << log_msg << std::endl; }
		void setLogTag(std::string& tag) {Tag = tag;}

	public:
//	Log() = delete;
		Log (std::string tag) : Tag(tag){}

};

/*
 * class Type
 */
 enum class Type {
	VARCHAR,
	INTEGER,
	DATE_TIME
};




#define getBitBoolVal(Val) (Val)?true:false
#define setBitBoolVal(Parm,Val) Parm=(Val)?1:0

 /*
  * class column_info
  */
class ColumnInfo {
public:

	 ColumnInfo(std::string &new_label, int new_size, Type new_type):
		 label(new_label),
		 size(new_size),
		 type(new_type)
	 {
		 setIsFk(false);
		 setIsPk(false);
		 setIsUnique(false);
//		 FK = nullptr;
	 }

//	const Table&& getFk() const {return FK;};
	bool getIsFk() const { return getBitBoolVal(isFK); };
	bool getIsPk() const { return getBitBoolVal(isPK); };
	bool getIsUnique() const { return getBitBoolVal(isUNIQUE); };
	const std::string& getLabel() const{ return label; } ;
	int getSize() const{return size;}
	Type getType() const{return type;}

	void setIsFk(bool Fk) {setBitBoolVal(isFK,Fk);};
	void setIsPk(bool Pk) {setBitBoolVal(isPK,Pk);};
	void setIsUnique(bool Unique) {setBitBoolVal(isPK,Unique);};

	void setLabel(std::string &new_label) { label = new_label; }
	void setSize(int new_size) {size=new_size;}
	void setType(Type new_type){type=new_type;}

private:
	std::string label;
	int size;
	Type type;
	unsigned char isPK:1;
	unsigned char isUNIQUE:1;
	unsigned char isFK:1;
//	Table FK;
};

/*
 * class row_info
 */
class RowInfo {
	ColumnInfo& column_info;
	std::any row_data;
public :
	RowInfo(ColumnInfo& new_column_info, std::any new_data = nullptr): column_info(new_column_info), row_data(new_data){}

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
class Table : public Log{
	std::string name;
	std::string schema;
	HeaderInfo header_info;
	RowData row_data;

		void setTableName(std::string& new_name) {
			name = new_name;
			updateLogTag();
		}
		void setSchemName(std::string& new_schema) {
			schema = new_schema;
			updateLogTag();
		}

		void updateLogTag(){
			Log::setLogTag(std::string(typeid(this).name()).append("_").append(name).append("_").append(schema));
		}


	public :
		Table () = delete;

		Table(const std::initializer_list<const char *> &list): Log(std::string("init")) {	//  :Log{std::ref(string("Table")}
			assert((list.size() > 0)&&"initializer_list is empty!");
			int count = 0;
			for (auto item: list) {
				switch (count) {
				case 0:
					name = *item;
					break;
				case 1:
					schema = *item;
					break;
				default:
					break;
				};
			}

			updateLogTag();
			msg("constructor initializer_list");
		}

		Table(const char * new_name, const char * new_schema) : Log(std::string("init")){	//  :Log{std::ref(string("Table")}
			assert(new_name && "initializer new name is empty!");
			name = new_name;
			if (new_schema) {schema = new_schema;}
			updateLogTag();
			msg("constructor parms");
		}

		Table(const char * new_name) :Table(new_name, nullptr) {	//  :Log{std::ref(string("Table")}
//			Table(new_name, nullptr);
		}


		void addColumn(ColumnInfo& col){

		}
};




class TabelControl{
	private:
		std::shared_ptr<pqxx::connection> db_connect;
		std::shared_ptr<db_tables_info>    tables_info; // @suppress("Invalid template argument")

		void RefreshTableList() {
			if (db_connect && (db_connect->is_open())) {
				pqxx::work statement{*db_connect};
				tables_info->clear();
				pqxx::result ret_data { statement.exec(LIST_TABLES) };
				for (auto row: ret_data) {
					tables_info->insert(db_table_info(row[0].as<std::string>(), row[1].as<std::string>()));
				}
			}
		}
	public:
		TabelControl(std::shared_ptr<pqxx::connection> connect) : db_connect(connect) {
			tables_info = std::make_shared<db_tables_info>();
			RefreshTableList();
		}

		std::shared_ptr<db_tables_info> getTableInfo(bool isUpdate) {
			if (isUpdate) {
				RefreshTableList();
			}
			return tables_info;
		}
};



}



#endif /* DB_DB_TABLE_CLASS_LIST_H_ */
