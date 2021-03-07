/*
 * consolInterfaceCmd.h
 *
 *  Created on: Feb 13, 2021
 *      Author: Agapov Aleksey
 */

#ifndef CONTROL_CONSOLINTERFACECMD_H_
#define CONTROL_CONSOLINTERFACECMD_H_

#define EXIT1 "^C"
#define EXIT2 "exit"
#define CLEAR "clear"
#define HELP "help"
#define LIST "list"
#define INFO "info"
#define SHOW "show"
#define SCHEMA "schema"
#define HISTORY "history"
#define SQL "sql"
#define CONFIG "config"
#define LOG "log"
#define SET "set"

#define REFRESH "refresh"
#define RECONNECT "reconnect"


#define ABOUT_TITLE "ABOUT APP PG_CLIENT"

inline static const char * EMPTY[] = {
		nullptr
};

inline static const char * EXIT_INFO[] = {
		"Exit from app.",
		"Use ^c for exit too.",
		"",
		nullptr
};

inline static const char * CLEAR_INFO[] = {
		"Clear buffer screen.",
		"",
		nullptr
};

inline static const char * HELP_INFO[] = {
		"This help.",
		"",
		nullptr
};
// //////////////////////////////////////////////////////////

inline static const char * LIST_INFO[] = {
		"Show list tables.",
		"",
		nullptr
};

inline static const char * INFO_INFO[] = {
		"Show info about selected table.",
		"FORMAT: info {<NUMBER_TABLE>|<TABLE_NAME>}",
		"",
		nullptr
};

inline static const char * SHOW_INFO[] = {
		"Show data selected tables.",
		"FORMAT: show {<NUMBER_TABLE_1> ... <NUMBER_TABLE n>|<TABLE_NAME_1> ... <TABLE_NAME_N>}",
		"             | <START_ROW> <ROW_NAMBERS_LIMIT>",
		"             | <COLUMN1> ... <COLUMNn> <COLUMN_BEGIN>-<COLUNM_END>",
		"",
		nullptr
};

inline static const char * SCHEMA_INFO[] = {
		"Set schema for fast access.",
		"FORMAT: schema <NEW_SCHEMA>",
		"",
		nullptr
};

inline static const char * HISTORY_INFO[] = {
		"Work to history.",
		"FORMAT: history <No argument>    - for list all history",
		"                <HISTORY_NUMBER> - for retry selected history",
		"",
		nullptr
};

inline static const char * SQL_INFO[] = {
		"Execute SQL.",
		"FORMAT: sql <SQL_COMMAND> - can be use SELECT, UPDATE, DELETE",
		"",
		nullptr
};

inline static const char * CONFIG_INFO[] = {
		"Control configuration.",
		"FORMAT: config load {<FULL_PATH_TO_FILE>|<FILE_NAME>}  - load config file",
		"               save {<FULL_PATH_TO_FILE>|<FILE_NAME>}  - save config file",
		"                     <FILE_NAME> - location in app path" ,
		"               edit <JSON_PARM_NAME> <JSON_PARM_VALUE> - edit config parametrs",
		"               show                                    - show all config parametrs",
		"               clear                                   - clear all config parametrs",
		"WARNING:   command clear erase all config value" ,
		"",
		nullptr
};

inline static const char * LOG_INFO[] = {
		"Control debugging ",
		"FORMAT: log    edit <JSON_PARM_NAME> <JSON_PARM_VALUE> - edit config parametrs",
		"               show                                    - show all config parametrs",
		"               clear                                   - clear all config parametrs",
		"WARNING:   command clear erase all config value" ,
		"",
		nullptr
};
inline static const char * SET_INFO[] = {
		"Select default module.",
		"FORMAT: set <No argument>    - return to root",
		"            {info|show|history|sql|config} - for retry selected history",
		"NOTE:   commands: set|exit|help|history|reconnect can run from any module" ,
		"",
		nullptr
};

inline static const char * REFRESH_INFO[] = {
		"Refresh table list.",
		"",
		nullptr
};

inline static const char * RECONNECT_INFO[] = {
		"Reconnect to db.",
		"",
		nullptr
};

#endif /* CONTROL_CONSOLINTERFACECMD_H_ */
