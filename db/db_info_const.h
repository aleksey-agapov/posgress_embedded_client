/*
 * db_info_const.h
 *
 *  Created on: Dec 8, 2020
 *      Author: Agapov Aleksey
 */

#ifndef DB_DB_INFO_CONST_H_
#define DB_DB_INFO_CONST_H_

#include <string>

namespace db {

#define LIST_TABLES "SELECT schemaname AS \"schema\", tablename  AS \"table\" FROM pg_catalog.pg_tables"

#define LIST_TABLES_COLUMN "SELECT table_schema AS \"schema\", table_name AS \"table\", column_name AS \"column\", ordinal_position AS \"position\", character_octet_length AS \"size\", is_nullable, data_type, udt_catalog, udt_schema, udt_name, is_updatable FROM information_schema.columns WHERE table_schema='%s' AND table_name='%s'"



#define LIST_TABLES_COLUMN_REF "SELECT z.table_schema AS \"schema\", z.table_name AS \"table\", z.column_name AS \"column\", z.constraint_type AS \"type\", x.table_schema AS \"fk_schema\", x.table_name AS \"fk_table\", x.column_name AS \"fk_column\" from \
(SELECT a.table_schema AS \"table_schema\", a.table_name AS \"table_name\", a.column_name AS \"column_name\", b.constraint_type AS \"constraint_type\", a.constraint_name AS \"constraint_name\" FROM information_schema.key_column_usage a, information_schema.table_constraints b WHERE b.constraint_name = a.constraint_name AND  a.table_schema='%s' AND a.table_name='%s') z left outer join \
(SELECT c.table_schema as \"table_schema\", c.table_name AS \"table_name\", c.column_name AS \"column_name\", a.constraint_name AS \"constraint_name\" FROM information_schema.key_column_usage a, information_schema.referential_constraints b, information_schema.key_column_usage c WHERE a.constraint_name = b.constraint_name AND b.unique_constraint_name = c.constraint_name ) x on z.constraint_name = x.constraint_name"




#define SCHEMA "schema"
#define TABLE "table"
#define COLUMN "column"
#define COLUMN_NUMBER "position"
#define COLUMN_SIZE "size"

#define IS_NULL "is_nullable"
#define IS_UPDATE "is_updatable"

#define DATA_TYPE1 "data_type"
#define DATA_TYPE2 "udt_name"

#define CONSTRAINT "type"

#define FK_SCHEMA "fk_schema"
#define FK_TABLE  "fk_table"
#define FK_COLUMN "fk_column"




#define CONST_FOREIGN_KEY "FOREIGN KEY"
#define CONST_PRIMARY_KEY "PRIMARY KEY"
// CHECK    UNIQUE

/*
 * class Type
 */
 enum class Type {
	TEXT,
	INTEGER,
	CHAR,
	FLOAT,
	BOOL,
	DATE,
	TIME,
	DATE_TIME,
	OTHER
};



class TypeInfo {
	 inline static const char *type_int[] = {
				"interval", "bytea", "smallint", "inet", "integer", "bigint", "xid", nullptr
		};

	 inline static const char *type_float[] = {
				"real", "double", nullptr
		};

	 inline static const char *type_string[] = {
				"text", "character", nullptr
		};

	 inline static const char *type_char[] = {
				"\"char\"", nullptr
		};

	 inline static const char *type_time[] = {
				"abstime", nullptr
		};

	 inline static const char *type_datetime[] = {
				"timestamp", nullptr
		};

	bool isType(std::string &type_name, const char *type_names[]) {
		bool ret = false;
		while (const char * index = static_cast<const char *>(*(type_names++))) {
			if (type_name.find(index) != std::string::npos) {
				ret = true;
				break;
			}
		}
		return ret;
	}

public:
	TypeInfo() {}
	Type getTypeNeme(std::string &type_name) {
		if (isType(type_name, type_int))       {return Type::INTEGER;}
		if (isType(type_name, type_float))     {return Type::FLOAT;}
		if (isType(type_name, type_string))    {return Type::TEXT;}
		if (isType(type_name, type_char))      {return Type::CHAR;}
		if (isType(type_name, type_time))      {return Type::TIME;}
		if (isType(type_name, type_datetime))  {return Type::DATE_TIME;}
		return Type::OTHER;
	}

};

}

#endif /* DB_DB_INFO_CONST_H_ */
