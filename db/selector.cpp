/*
 * selector.cpp
 *
 *  Created on: Dec 28, 2020
 *      Author: user
 */

#include "selector.h"

namespace db {

selector::selector(vector<std::shared_ptr<InterfaceTable> > new_table_list): Log("selector"), table_list(new_table_list){
	if(!table_list.empty()){
		int count = 0;
		std::for_each(std::begin(table_list), std::end(table_list), [&](std::shared_ptr<InterfaceTable>& table) {
			if (count) {
				from << ",";
			}
			calc_column(count);
			from << table->getSchema() << "." << table->getName() << " T"  << count++;
		}
		);

		if ((!from.str().empty()) && (!fields.str().empty())) {
			select_sql << "SELECT "<< fields.str() << " FROM "<< from.str();
			if (!where.str().empty()) {
				select_sql << " WHERE "<< where.str();
			}
			msg(select_sql.str().c_str());
		}
	}
}

/*

selector& selector::operator=(const selector &other) {
	// TODO Auto-generated method stub

}

selector& selector::operator=(selector &&other) {
	// TODO Auto-generated method stub

}
*/
} /* namespace db */
