/*
 * CommandControl.h
 *
 *  Created on: Nov 14, 2020
 *      Author: root
 */

#ifndef CONTROL_COMMANDCONTROL_H_
#define CONTROL_COMMANDCONTROL_H_

#include <iostream>
#include <map>
#include <memory>
#include <cstring>
#include <functional>
#include "Log.h"

namespace control {

	class About;
	class History;
	class DefaultSchema;

	typedef std::function<void (std::string& cmd_line)> intercept_func;  // (*)
	typedef std::shared_ptr<intercept_func> ptr_action_func;
	typedef std::map<const char *, ptr_action_func > action_list;

	class RunCmdInterface {
		// Делаем класс History другом класса CommandControl
		friend class control::History;

		virtual void run_cmd(std::string cmd_line, bool isHistory) = 0;
	public:
		virtual ~RunCmdInterface() = default;
	};




	class IsDefaultCmdInterface {
		// Делаем класс History другом класса CommandControl
		friend class control::DefaultSchema;
		virtual bool isDefaultSchema(std::string cmd_line) = 0;
	public:
		virtual ~IsDefaultCmdInterface() = default;
	};



	class CommandControl final : protected Log, public RunCmdInterface, public IsDefaultCmdInterface, std::enable_shared_from_this<RunCmdInterface>, std::enable_shared_from_this<IsDefaultCmdInterface>  {
	public:
		CommandControl();
		virtual ~CommandControl() = default;

		CommandControl& operator=(const CommandControl &other){
			// TODO Auto-generated constructor stub
			return *this;
		}
		CommandControl& operator=(CommandControl &&other){
			// TODO Auto-generated constructor stub
			return *this;
		}

		void Start();
		void RegisterAbout(std::shared_ptr<control::About> about_class);
		void RegisterSetDefault(std::shared_ptr<control::DefaultSchema> default_class, const char ** ignoreDefaultCmd);
		void RegisterHistory(std::shared_ptr<control::History> new_history, std::string history_cmd ) ;
		void RegisterOperation(const char * Key, ptr_action_func &ptr_func, bool isDefault, const char ** about_info);
		void RegisterDefault(ptr_action_func &ptr_func);

	private:
		using isDefaultHash = std::map<const char *, bool>;

		isDefaultHash is_default_list;
		std::string history_token;
		std::shared_ptr<control::History> history;
		std::shared_ptr<control::DefaultSchema> defaultClass;
		std::shared_ptr<control::About> aboutClass;
		ptr_action_func default_func;
		action_list cmd_list;

		const char ** ignoreDefaultCmdList;

		bool isInRegisterList(const char * key);
		bool isDefaultSchema(std::string cmd_line) ;
		void run_cmd(std::string cmd_line, bool isHistory) ;
	};

} /* namespace control */

#endif /* CONTROL_COMMANDCONTROL_H_ */

