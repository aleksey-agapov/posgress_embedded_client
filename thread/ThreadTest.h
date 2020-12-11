/*
 * ThreadTest.h
 *
 *  Created on: Nov 16, 2020
 *      Author: root
 */

#ifndef THREAD_THREADTEST_H_
#define THREAD_THREADTEST_H_

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace threads {

class ThreadTest {
private:
	std::mutex m;
	std::condition_variable control_lock;
	volatile bool isLock;
	volatile bool isRun;
public:
	ThreadTest():isLock(true), isRun(false) {
	}
	void start_thread() {

		// Wait until main() sends data
		std::unique_lock<std::mutex> lk(m);
		isRun = true;
		while (isRun)  {
			while (isLock) {
				control_lock.wait(lk);
			}
			std::cout << "Work!!! \r" << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(2));
		}
	}

	void pause() {
		isLock = true;
	}

	void resume() {
		isLock = false;
		control_lock.notify_all();
	}

    void operator()()
    {
    	start_thread();
    }

};

} /* namespace thread */

#endif /* THREAD_THREADTEST_H_ */
