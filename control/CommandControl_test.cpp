/*
 * CommandControl_test.cpp
 *
 *  Created on: Nov 14, 2020
 *      Author: root
 */
#include <cassert>
#include "CommandControl.h"

namespace control {
	void Tests(){
		assert((new CommandControl()!=nullptr)&&"initializer_list is empty!");
	}
} /* namespace control */
