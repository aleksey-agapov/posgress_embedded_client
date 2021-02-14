/*
 * About.cpp
 *
 *  Created on: Feb 14, 2021
 *      Author: user
 */

#include <iostream>
#include "About.h"
#include "../gui/OutputForm.h"

namespace control {

About::About(const char * title){
	help = std::make_unique<gui::OutputForm> ();
	help->setTitle(title);
	help->add_column("command").add_column("description");
}

void About::AddInfo(const char * Key, const char ** about_info) {
	bool isFirst = true;
	while (*about_info) {
		if (isFirst) {
			help->add_row({Key,*about_info}); isFirst= false;
		} else {
			help->add_row({"",*about_info});
		}
		about_info++;
	}
}

void About::printAbout(std::ostream& out_stream) {
	gui::printer show_list(std::cout);
	help = std::move(show_list.showForm(std::move(help)));
}


} /* namespace control */
