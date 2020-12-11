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




	class CommandControl final {
	public:
		CommandControl() {
			// TODO Auto-generated constructor stub
			isStart = false;
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
			const std::string delimiter(" ");
			isStart = true;
			while (isStart) {
				std::string token;
				std::string command_string("");
				std::cout << "Read cmd>>";
				std::getline(std::cin, cmd_line);
				trim(cmd_line);

				std::string::size_type first_pos = cmd_line.find(delimiter);
				if (first_pos == std::string::npos) {
					first_pos = cmd_line.size();
				} else {
					command_string = cmd_line.substr(first_pos + 1); // token is "scott"
				}

				token = cmd_line.substr(0, first_pos); // token is "scott"
				std::cout << ">> Read cmd >> " << token << " >> " << command_string << std::endl;
/*
				 auto it = cmd_list.find(token.c_str());
				  if (it != cmd_list.end()) {
					  std::cout << "FOUND!!!!!" << std::endl;
					  (*it->second)(command_string);
				  }
*/

				for (const auto &pair : cmd_list) {
					if (strcmp(pair.first,token.c_str()) == 0) {
						(*pair.second)(command_string);
						break;
					}
				}
			}
		}
		void Stop(){
			// TODO Auto-generated constructor stub
			isStart = false;
		}
		int RegisterOperation(const char * Key, ptr_action_func &ptr_func){
			// TODO Auto-generated constructor stub
			cmd_list[Key] = ptr_func;

			return 0;
		}

	private:
		action_list cmd_list;
		bool isStart;
	};

} /* namespace control */

#endif /* CONTROL_COMMANDCONTROL_H_ */

