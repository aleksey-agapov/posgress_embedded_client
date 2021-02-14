/*
 * DefaultSchema.cpp
 *
 *  Created on: Jan 31, 2021
 *      Author: user
 */

#include "DefaultSchema.h"
#include "CommandControl.h"

namespace control {

DefaultSchema::DefaultSchema() : default_module(""), controlDefaultInterface(nullptr) {}

std::string DefaultSchema::getDefaultModule(){
	return default_module;
}

bool DefaultSchema::setDefault(std::string& in){
	bool isDefault = false;
	if (in.empty()) {isDefault = true;}
	else {
		isDefault = controlDefaultInterface->isDefaultSchema(in);
	}
	if (isDefault) {
		default_module = in;
	}
	return isDefault;
}

void DefaultSchema::setDefaultInterfaceListner(std::shared_ptr<IsDefaultCmdInterface> controlDefaultInterface) {
	this->controlDefaultInterface = controlDefaultInterface;
}

} /* namespace control */
