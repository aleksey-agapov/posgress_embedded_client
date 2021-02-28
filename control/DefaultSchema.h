/*
 * DefaultSchema.h
 *
 *  Created on: Jan 31, 2021
 *      Author: Agapov Aleksey
 */

#ifndef CONTROL_DEFAULTSCHEMA_H_
#define CONTROL_DEFAULTSCHEMA_H_
#include <string>
#include <memory>
namespace control {

class IsDefaultCmdInterface;

class DefaultSchema final  {
private:
	std::string default_module;
	std::shared_ptr<IsDefaultCmdInterface> controlDefaultInterface;
public:
	DefaultSchema();
	virtual ~DefaultSchema() = default;
	std::string getDefaultModule();
	int getCommandType(std::string& in);
	bool setDefault(std::string& in);
	void setDefaultInterfaceListner(std::shared_ptr<IsDefaultCmdInterface> controlDefaultInterface);
};

} /* namespace config */

#endif /* CONTROL_DEFAULTSCHEMA_H_ */
