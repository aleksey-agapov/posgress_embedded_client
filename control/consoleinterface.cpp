/*
 * consoleinterface.cpp
 *
 *  Created on: Nov 15, 2020
 *      Author: root
 */

#include "db_table_class_list.h"

using namespace std;

//namespace control {
#include <iostream>

void exit_func(std::string& in) {
	std::cout << in << " EXIT!!!" << std::endl;
	exit(0);
}

void clear_func (std::string& in) {
	// CSI[2J clears screen, CSI[H moves the cursor to top-left corner
	std::cout << "\x1B[2J\x1B[H\033c";
//	clrscr();
}

void help_func (std::string& in) {
	clear_func(in);
	std::cout << "+---------------------------------------------------------------+" << std::endl;
	std::cout << "|-resume--------------------------------------------------------|" << std::endl;
	std::cout << "|-pause---------------------------------------------------------|" << std::endl;
	std::cout << "|---------------------------------------------------------------|" << std::endl;
	std::cout << "|-clear---------------------------------------------------------|" << std::endl;
	std::cout << "|-help----------------------------------------------------------|" << std::endl;
	std::cout << "|-list_table----------------------------------------------------|" << std::endl;
	std::cout << "|-exit----------------------------------------------------------|" << std::endl;
	std::cout << "+---------------------------------------------------------------+" << std::endl;
}
//} /* namespace control */
