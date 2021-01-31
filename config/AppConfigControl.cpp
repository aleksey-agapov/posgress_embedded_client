/*
 * AppConfigControl.cpp
 *
 *  Created on: Jan 27, 2021
 *      Author: user
 */

#include "AppConfigControl.h"
#include <fstream>      // std::filebuf

namespace config {

AppConfigControl::AppConfigControl(std::string file_config): file_config(file_config) {
	// TODO Auto-generated constructor stub

}

void AppConfigControl::LoadNewConfig(const std::string FileName) {
	std::error_code errorCode;
	std::filebuf fb;
	if (fb.open(FileName.c_str(), std::ios::in)) {
		try {
			std::cout << "!!!!!!!!!!READ!!!!!!!!!" << FileName
					<< "!!!!!!!!!!READ!!!!!!!!!" << std::endl;
			utility::istream_t input(&fb); // = static_cast<utility::istream_t&>(read_config_stream);
			base_config = json::value::parse(input, errorCode); // @suppress("Function cannot be resolved")
		} catch (const web::json::json_exception &e) {
			std::cerr << "JSON_ERROR:" << e.what() << std::endl;
		} catch (std::exception &e) {
			std::cerr << "ERROR:" << e.what() << std::endl;
			// Return an empty task.
		}
	}
	try {
		fb.close();
	} catch (std::exception &e) {
		std::cerr << "ERROR:" << e.what() << std::endl;
		// Return an empty task.
	}
}



void AppConfigControl::SaveConfig(const std::string FileName) {
	std::error_code errorCode;
	std::filebuf fb;
	if (fb.open(FileName.c_str(), std::ios::out)) {
		try {
			std::cout << "!!!!!!!!!!UPDATE!!!!!!!!!" << FileName
					<< "!!!!!!!!!!UPDATE!!!!!!!!!" << std::endl;
			utility::ostream_t output(&fb); // = static_cast<utility::istream_t&>(read_config_stream);
			output << base_config.serialize() << std::endl; // = json::value::se(input, errorCode); // @suppress("Function cannot be resolved") // @suppress("Method cannot be resolved") // @suppress("Invalid overload")
		} catch (const web::json::json_exception &e) {
			std::cerr << "JSON_ERROR:" << e.what() << std::endl;
		} catch (std::exception &e) {
			std::cerr << "ERROR:" << e.what() << std::endl;
			// Return an empty task.
		}
	}
	try {
		fb.close();
	} catch (std::exception &e) {
		std::cerr << "ERROR:" << e.what() << std::endl;
		// Return an empty task.
	}
}



AppConfigControl::~AppConfigControl() {
	// TODO Auto-generated destructor stub
}

} /* namespace config */





/*
			// тест строк
			web::json::value color = json_forma_config.at(U("color"));
			wcout << U("string:") << color.as_string().c_str() << std::endl;

			// тест массивов
			web::json::value array = json_forma_config.at(U("array"));
			int sizeArray = array.as_array().size();
			for (int count = 0;count < sizeArray;count++) {
				wcout << U("array  index:") << count << U(" value:") << array[count].as_integer() << std::endl;
			}



			// Note the "as_object()" method calls

			for (auto iter = json_forma_config.as_object().cbegin(); iter != json_forma_config.as_object().cend(); ++iter)

			{

				// This change lets you get the string straight up from "first"

				const utility::string_t& str = iter->first;

				const web::json::value& v = iter->second;

				wcout << str << U("===>") << v << std::endl;

			}

*/

