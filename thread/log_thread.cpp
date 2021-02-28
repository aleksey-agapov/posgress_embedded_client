/*
 * ThreadTest.cpp
 *
 *  Created on: Nov 16, 2020
 *      Author: Agapov Aleksey
 */
#include <thread>
#include <iostream>
#include <filesystem>

#include "log_thread.h"

namespace threads {

void LogThread::start_thread() {
	// Wait until main() sends data
	std::unique_lock<std::mutex> lk(m);
	isRun = true;
	while (isRun)  {
		int count = 0;

		while (isLock) {
//			std::cerr << "THREAD LOCK!" << std::endl;
			control_lock.wait(lk);
		}

		while(!log_queue.empty()) {
			if (writeLogMsg(log_queue.front().c_str())) {
				log_queue.pop_front();
				count ++;
			} else {
				break;
			}
		}

//		std::cerr << "Loger write:" << count << std::endl;
//		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
//	std::cerr << "THREAD STOP!" << std::endl;
}

void LogThread::setConfigControl(std::shared_ptr<config::LogAppConfig> log_config) {
	this->log_config = log_config;
	log_config->RegisterCallback(log_config->getModuleName(), this);  // std::shared_ptr<config::IConfigCallBack>(this)
}


void LogThread::Update() {
	log_config->Update();
	setNewFile(log_config->getLogFilePath(), log_config->isDebug());

	if (isDebug) {resume();}
	else {pause();}
}


void LogThread::Destroy(){
	isRun = false;
	resume();
	std::this_thread::sleep_for(std::chrono::seconds(4));
}

void LogThread::start_loger()
{
	std::thread log_thread([this]{start_thread();}); // t5 runs foo::bar() on object f
	log_thread.detach();
}

void LogThread::close_log_file() {
	if (output_file.is_open()) {
		try {
			output_file.close();
		} catch (...) {}
	}
}


void LogThread::SetLogFilePath(std::string FileName) {
	std::string output_file_name;
	if (std::filesystem::path(FileName).is_absolute()) {
		output_file_name = FileName;
	} else {
		output_file_name.clear();
		output_file_name.append( std::filesystem::current_path().c_str() ).append("/").append(FileName);
	}
	close_log_file();
	output_file.open(output_file_name, std::ofstream::out | std::ofstream::app); // текстовый файл для вывода
	if (!output_file.is_open()) {
		std::cerr << "Cannot open the file:" << output_file_name << " to output." << std::endl;
	}
}

bool LogThread::writeLogMsg(const char *output_str) {
	bool isWrite = false ;
	if (isWriteLog()) {
		// проверка, открыт ли файл
		if (!output_file.is_open()) {
			return false;
		}
		try {
			output_file << output_str << std::endl;
			isWrite = true;
		} catch (std::exception &ex) {
			std::cerr << "Error of write to log file." << ex.what() << std::endl;
		}
	} else {
		isWrite = true;
	}
	return isWrite;
}


} /* namespace control */
