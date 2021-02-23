/*
 * log_thread.h
 *
 *  Created on: Nov 16, 2020
 *      Author: root
 */

#ifndef THREAD_LOG_THREAD_H_
#define THREAD_LOG_THREAD_H_


#include <string>
#include <mutex>
#include <deque>
#include <memory>
#include <condition_variable>
#include <fstream>      // std::ofstream

#include "../config/AppConfigControl.h"

namespace threads {

#define MAX_QUEUE 10
#define DEFAULT_FILE_NAME "log_file.log"

class LogThread : public config::IConfigCallBack, std::enable_shared_from_this<config::IConfigCallBack> {
private:
	std::mutex m;
	std::condition_variable control_lock;
	std::shared_ptr<config::LogAppConfig> log_config;
	std::ofstream output_file;
//	std::string output_file_name;

	volatile bool isLock = true;
	volatile bool isRun  = false;

	inline static bool isDebug = false;
	inline static std::deque<std::string> log_queue;


void SetLogFilePath(std::string FileName) ;
void start_thread();
void close_log_file();
bool writeLogMsg(const char *output_str) ;

void setNewFile(std::string new_file, bool isNewStatus) {
	SetLogFilePath( new_file);
	isDebug = isNewStatus;
}


public:
	LogThread(std::string log_name) { SetLogFilePath(log_name); }
	LogThread(): LogThread(DEFAULT_FILE_NAME){}
	virtual ~LogThread() {
		close_log_file() ;
	}

	void setConfigControl(std::shared_ptr<config::LogAppConfig> log_config) ;
	void Update() ;

	static void setLogMsg (std::string input) {
		log_queue.push_back(input);
	}

	static bool isWriteLog() {
		return isDebug;
	}

	void pause() {
		isLock = true;
	}

	void resume() {
		isLock = false;
		control_lock.notify_all();
	}

	void start_loger();

    void send_log (std::string& log_msg){
    	log_queue.push_back(log_msg);
    }

    void Destroy();

};

} /* namespace thread */

#endif /* THREAD_LOG_THREAD_H_ */
