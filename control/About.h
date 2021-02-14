/*
 * About.h
 *
 *  Created on: Feb 14, 2021
 *      Author: user
 */

#ifndef CONTROL_ABOUT_H_
#define CONTROL_ABOUT_H_

#include <memory>

namespace gui {
	class OutputForm;
}



namespace control {

class About final {
public:
	About(const char * title);
	void AddInfo(const char * Key, const char ** about_info);
	void printAbout(std::ostream& out_stream);

	virtual ~About() = default;
private:
	std::unique_ptr<gui::OutputForm> help;
};

} /* namespace control */

#endif /* CONTROL_ABOUT_H_ */
