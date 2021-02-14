/*
 * consolInterfaceCmd.h
 *
 *  Created on: Feb 13, 2021
 *      Author: user
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
#define SET "set"


#define ABOUT_TITLE "ABOUT APP PG_CLIENT"

inline static const char * EMPTY[] = {
		nullptr
};

inline static const char * EXIT_INFO[] = {
		"Exit from app.",
		"Use ^c for exit too.",
		nullptr
};

inline static const char * CLEAR_INFO[] = {
		"Clear buffer screen.",
		nullptr
};

inline static const char * HELP_INFO[] = {
		"This help.",
		nullptr
};
// //////////////////////////////////////////////////////////

inline static const char * LIST_INFO[] = {
		"Show list tables.",
		nullptr
};

inline static const char * INFO_INFO[] = {
		"Show info about selected table.",
		nullptr
};

inline static const char * SHOW_INFO[] = {
		"Show tables row.",
		nullptr
};

inline static const char * SCHEMA_INFO[] = {
		"Set schema for fast access.",
		nullptr
};

inline static const char * HISTORY_INFO[] = {
		"Work to history.",
		nullptr
};

inline static const char * SQL_INFO[] = {
		"Execute SQL.",
		nullptr
};

inline static const char * CONFIG_INFO[] = {
		"Control configuration.",
		nullptr
};

inline static const char * SET_INFO[] = {
		"Select default module.",
		nullptr
};




#endif /* CONTROL_CONSOLINTERFACECMD_H_ */
