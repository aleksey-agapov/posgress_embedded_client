/*
 * Log.h
 *
 *  Created on: Feb 17, 2021
 *      Author: Agapov Aleksey
 */

#ifndef CONTROL_LOG_H_
#define CONTROL_LOG_H_

#include <iostream>
#include <string>
#include <sstream>      // std::ostringstream

namespace control {

template <class T>
std::string to_str(const T &val)
{
  return std::to_string(val);
}

inline std::string to_str(const char *p)
{
  return p;
}

inline const std::string& to_str(const std::string &s)
{
  return s;
}


/*
 * class Log
 */
class Log {
	std::string Tag;

	static void sendToDeamon(const std::string out_msg);
	static bool isWriteLog();

	public:
		Log() = delete;
		Log (std::string tag) : Tag(tag){ msg("constructor");}
		~Log() {msg("destructor");}

		void setLogTag(std::string tag) {Tag = {tag};}

		template<typename ... Args>
		void msg(Args ...params) {
			std::ostringstream loger_msg_stream;
			std::string param_list[] { "", to_str(params)... };

			loger_msg_stream << Tag << " ==> ";

			for (std::string & parm_current: param_list) {
				loger_msg_stream << parm_current;
			}

//			std::cerr << loger_msg_stream.str() << std::endl;
			sendToDeamon(loger_msg_stream.str());
		}

	    template<typename T>
		void msg(T log_msg) {
			if (isWriteLog()) {
				std::ostringstream loger_msg_stream;
				loger_msg_stream << Tag << " ==> " << log_msg;
//				std::cerr << loger_msg_stream.str() << std::endl;
				sendToDeamon(loger_msg_stream.str());
			}
		}
};

} /* namespace control */

#endif /* CONTROL_LOG_H_ */
