/*
 * Log.cpp
 *
 *  Created on: Feb 17, 2021
 *      Author: Agapov Aleksey
 */

#include "Log.h"
#include "../thread/log_thread.h"

namespace control {

void Log::sendToDeamon(const std::string out_msg) {
	threads::LogThread::setLogMsg(out_msg);
}

bool Log::isWriteLog() {
	return threads::LogThread::isWriteLog();
}

} /* namespace control */
