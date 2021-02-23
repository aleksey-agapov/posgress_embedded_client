/*
 * Log.cpp
 *
 *  Created on: Feb 17, 2021
 *      Author: user
 */


//#include <thread>

#include "Log.h"
#include "../thread/log_thread.h"

namespace control {

void Log::sendToDeamon(const std::string out_msg) {
	threads::LogThread::setLogMsg(out_msg);
}

} /* namespace control */
