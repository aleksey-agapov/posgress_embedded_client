/*
 * db_info_const.h
 *
 *  Created on: Dec 8, 2020
 *      Author: root
 */

#ifndef DB_DB_INFO_CONST_H_
#define DB_DB_INFO_CONST_H_

namespace db {

#define LIST_TABLES "SELECT schemaname AS \"schema\", tablename  AS \"table\" FROM pg_catalog.pg_tables"

#define LIST_TABLES_COLUMN "SELECT table_schema AS \"schema\", table_name AS \"table\", column_name AS \"column\", ordinal_position AS \"position\", is_nullable, data_type, udt_catalog, udt_schema, udt_name, is_updatable FROM information_schema.columns WHERE table_schema='%s' AND table_name='%s';"



#define LIST_TABLES_COLUMN_REF "SELECT z.table_schema AS \"schema\", z.table_name AS \"table\", z.column_name AS \"column\", z.constraint_type AS \"type\", x.table_schema AS \"fk_schema\", x.table_name AS \"fk_table\", x.column_name AS \"fk_column\" from \
(SELECT a.table_schema AS \"table_schema\", a.table_name AS \"table_name\", a.column_name AS \"column_name\", b.constraint_type AS \"constraint_type\", a.constraint_name AS \"constraint_name\" FROM information_schema.key_column_usage a, information_schema.table_constraints b WHERE b.constraint_name = a.constraint_name AND  a.table_schema='%s' AND a.table_name='%s') z left outer join \
(SELECT c.table_schema as \"table_schema\", c.table_name AS \"table_name\", c.column_name AS \"column_name\", a.constraint_name AS \"constraint_name\" FROM information_schema.key_column_usage a, information_schema.referential_constraints b, information_schema.key_column_usage c WHERE a.constraint_name = b.constraint_name AND b.unique_constraint_name = c.constraint_name ) x on z.constraint_name = x.constraint_name;"




#define SCHEMA "schema"
#define TABLE "table"
#define COLUMN "column"
#define COLUMN_NUMBER "position"

#define IS_NULL "is_nullable"
#define IS_UPDATE "is_updatable"

#define DATA_TYPE1 "data_type"
#define DATA_TYPE2 "udt_name"

#define CONSTRAINT "type"

#define FK_SCHEMA "fk_schema"
#define FK_TABLE  "fk_table"
#define FK_COLUMN "fk_column"
}

#endif /* DB_DB_INFO_CONST_H_ */
