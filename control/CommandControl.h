/*
 * CommandControl.h
 *
 *  Created on: Nov 14, 2020
 *      Author: root
 */

#ifndef CONTROL_COMMANDCONTROL_H_
#define CONTROL_COMMANDCONTROL_H_
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <cstring>
#include "History.h"

namespace control {

	typedef std::function<void (std::string& cmd_line)> intercept_func;  // (*)
	typedef std::shared_ptr<intercept_func> ptr_action_func;
	typedef std::map<const char *, ptr_action_func > action_list;


	// trim from start (in place)
	static inline void ltrim(std::string &s) {
	    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
	        return !std::isspace(ch);
	    }));
	}

	// trim from end (in place)
	static inline void rtrim(std::string &s) {
	    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
	        return !std::isspace(ch);
	    }).base(), s.end());
	}

	// trim from both ends (in place)
	static inline void trim(std::string &s) {
	    ltrim(s);
	    rtrim(s);
	}
	// to lower case
	static inline std::string& to_lower(std::string& str)
	{
	    std::transform(str.begin(), str.end(), str.begin(), static_cast<int(*)(int)>(std::tolower));
	    return str;
	}

	// to upper case
	static inline std::string& to_upper(std::string& str)
	{
	    std::transform(str.begin(), str.end(), str.begin(), static_cast<int(*)(int)>(std::toupper));
	    return str;
	}



	class RunCmdInterface {
		// Делаем класс History другом класса CommandControl
		friend class History;

		virtual void run_cmd(std::string cmd_line, bool isHistory) = 0;
	public:
		virtual ~RunCmdInterface() {}
	};




	class CommandControl final : public RunCmdInterface,  std::enable_shared_from_this<RunCmdInterface>  {
	public:
		CommandControl(): default_func(nullptr), isStart(false) {
			// TODO Auto-generated constructor stub

		}
		virtual ~CommandControl() {
			// TODO Auto-generated destructor stub
			cmd_list.clear();
		}
		CommandControl(CommandControl &&other) : CommandControl(){
			// TODO Auto-generated constructor stub
//			CommandControl();
		}
		CommandControl& operator=(const CommandControl &other){
			// TODO Auto-generated constructor stub
			return *this;
		}
		CommandControl& operator=(CommandControl &&other){
			// TODO Auto-generated constructor stub
			return *this;
		}
		CommandControl(const CommandControl &other): CommandControl(){
			// TODO Auto-generated constructor stub
//			CommandControl();
		}
		void Start(){
			// TODO Auto-generated constructor stub
			std::string cmd_line;
			isStart = true;
			while (isStart) {

				if (std::cin.fail()) // если предыдущее извлечение было неудачным,
				{
				    std::cin.clear(); // то возвращаем cin в 'обычный' режим работы
				    std::cin.ignore(32767,'\n'); // и удаляем значения предыдущего ввода из входного буфера
				}

				std::cout << "Read cmd>>";
				std::getline(std::cin, cmd_line);
				trim(cmd_line);

				if (!cmd_line.empty()) {
					run_cmd(cmd_line, false);
				}

			}
		}
		void Stop(){
			// TODO Auto-generated constructor stub
			isStart = false;
		}



		void RegisterHistory(std::shared_ptr<History> new_history, std::string history_cmd ) {
			this->history = new_history;
			this->history_token = history_cmd;
			std::shared_ptr<control::RunCmdInterface> pp = std::shared_ptr<control::RunCmdInterface>(this);
			this->history->add_cmd_interface(pp); // shared_from_this()
		}


		void RegisterOperation(const char * Key, ptr_action_func &ptr_func){
			// TODO Auto-generated constructor stub
			cmd_list[Key] = ptr_func;
		}

		void RegisterDefault(ptr_action_func &ptr_func){
			default_func = ptr_func;
		}

	private:
		std::string history_token;
		std::shared_ptr<History> history;
		ptr_action_func default_func;
		action_list cmd_list;
		bool isStart;


		void run_cmd(std::string cmd_line, bool isHistory) {
			std::string token;
			std::string command_string;
			const std::string delimiter(" ");

			std::string::size_type first_pos = cmd_line.find(delimiter);
			if (first_pos == std::string::npos) {
				token = cmd_line;
			} else {
				command_string = cmd_line.substr(first_pos + 1); // token is "scott"
				token = cmd_line.substr(0, first_pos); // token is "scott"
			}

			bool isDefault = true;
			for (const auto &pair : cmd_list) {
				if (strcmp(pair.first,token.c_str()) == 0) {
					(*pair.second)(command_string);

					isDefault = false;

					if (!isHistory ) {
						if (token.find(history_token) == std::string::npos) {
							if (!history->addValue(cmd_line)){
								std::cerr << "Error add command in History" << std::endl;
							}
						}
					}
					break;
				}
			}
			if (isDefault && default_func) {
				(*default_func)(cmd_line);
			}
		}

	};

} /* namespace control */

#endif /* CONTROL_COMMANDCONTROL_H_ */

