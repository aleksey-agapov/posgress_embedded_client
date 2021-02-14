/*
 * AppConfigControl.cpp
 *
 *  Created on: Jan 27, 2021
 *      Author: user
 */

#include "AppConfigControl.h"
#include <fstream>      // std::filebuf
#include <filesystem>
#include <sstream>
#include <utility>
#include "../db/db_table_class_list.h"



namespace config {

AppConfigControl::AppConfigControl(std::string file_config) {
	// TODO Auto-generated constructor stub
	LoadNewConfig(file_config.c_str());
}

std::unique_ptr<std::string> AppConfigControl::showTree(const char * config_module) {
	std::stringstream output;
	try {
		if (!base_config.is_null() && (base_config.size() > 0)) {
			if (config_module && base_config.has_field(config_module)) {
				output << base_config.at(config_module).serialize() << std::endl;
			} else {
				output << base_config.serialize() << std::endl;
			}
		}
	} catch(...) {};
	return std::move( std::make_unique<std::string>( output.str() ) );
}



void AppConfigControl::LoadNewConfig(const char * FileName) {
	std::error_code errorCode;
	std::filebuf fb;
	file_config.clear();
	base_config = json::value::object();
	if (std::filesystem::path(FileName).is_absolute()) {
		file_config = FileName;
	} else {
		file_config.append( std::filesystem::current_path().c_str() ).append("/").append(FileName);
	}
	if (fb.open(file_config.c_str(), std::ios::in)) {
		try {
			std::cout << "!!!!!!!!!!READ!!!!!!!!!" << file_config
					<< "!!!!!!!!!!READ!!!!!!!!!" << std::endl;
			utility::istream_t input(&fb); // = static_cast<utility::istream_t&>(read_config_stream);
			base_config = json::value::parse(input, errorCode); // @suppress("Function cannot be resolved")

			this->Update();

		} catch (const web::json::json_exception &e) {
			std::cerr << "JSON_ERROR:" << e.what() << std::endl;
		} catch (std::exception &e) {
			std::cerr << "ERROR:" << e.what() << std::endl;
			// Return an empty task.
		}

		try {
			fb.close();
		} catch (...) {
			// Return an empty task.
		}
	} else {
		std::cerr << "ERROR in read :" << file_config << std::endl;
	}
}



void AppConfigControl::SaveConfig(const char * FileName) {
	std::error_code errorCode;
	std::filebuf fb;
	std::string full_path;

	if (std::filesystem::path(FileName).is_absolute()) {
		full_path = FileName;
	} else {
		full_path.append( std::filesystem::current_path().c_str() ).append("/").append(FileName);
	}

	if (fb.open(full_path.c_str(), std::ios::out)) {
		try {
			std::cout << "!!!!!!!!!!UPDATE!!!!!!!!!" << full_path
					<< "!!!!!!!!!!UPDATE!!!!!!!!!" << std::endl;
			utility::ostream_t output(&fb); // = static_cast<utility::istream_t&>(read_config_stream);
			output << base_config.serialize() << std::endl; // = json::value::se(input, errorCode); // @suppress("Function cannot be resolved") // @suppress("Method cannot be resolved") // @suppress("Invalid overload")
		} catch (const web::json::json_exception &e) {
			std::cerr << "JSON_ERROR:" << e.what() << std::endl;
		} catch (std::exception &e) {
			std::cerr << "ERROR:" << e.what() << std::endl;
			// Return an empty task.
		}

		try {
			fb.close();
		} catch (...) {
			// Return an empty task.
		}
	} else {
		std::cerr << "ERROR in write :" << full_path << std::endl;
	}
}

void AppConfigControl::setModuleConfig(const char * section_name, json::value& section_value) {
	base_config[U(section_name)] = section_value;
	SaveDefault();
}


void AppConfigControl::SaveDefault() {
	SaveConfig(file_config.c_str());
}


bool AppConfigControl::isContainModule (const char * KeyName) const{
	return (!this->isEmpty() && base_config.has_field(KeyName)) ;
}


json::value AppConfigControl::getSectionConfig(const char * section_name) {
	json::value section_config;
	if (this->isContainModule(section_name)) {
		section_config = base_config.at(U(section_name));
	} else {
		section_config = json::value::object();
	}
	return section_config;
}

void AppConfigControl::RegisterCallback(const char * key, std::shared_ptr<IConfigCallBack> callback){
//	callbackConnection.insert({key, callback}); // std::make_pair<const char *, config::IConfigCallBack*>(   //
	callbackConnection[key] = callback;
	callbackConnection[key]->Update();
}


void AppConfigControl::Update() {
	for (std::map<const char *, std::shared_ptr<IConfigCallBack>>::iterator callback_record = std::begin(callbackConnection);
			callback_record != std::end(callbackConnection);
			callback_record++) {
		std::cerr << "KEY:" << callback_record->first << std::endl;
		callback_record->second->Update();
	}
}


void AppConfigControl::UnRegisterCallback(const char * key ){
	callbackConnection.erase(key);
}

void        AppConfig::saveSession()                                        { setModuleConfig(section_config, section_config_json);}
bool        AppConfig::isSectionEmpty()                    const            { return section_config_json.size() == 0;};
bool        AppConfig::isContainKey (const char * KeyName) const            { return (!isSectionEmpty() && section_config_json.has_field(KeyName));}

void        AppConfig::setBool(const char * key, const bool value)          {section_config_json[U(key)] = json::value::boolean(value);  setModuleConfig(section_config, section_config_json);}
void        AppConfig::setDouble(const char * key, const double value)      {section_config_json[U(key)] = json::value::number(value);   setModuleConfig(section_config, section_config_json);}
void        AppConfig::setInt(const char * key, const int value)            {section_config_json[U(key)] = json::value::number(value);   setModuleConfig(section_config, section_config_json);}
void        AppConfig::setString(const char * key, const char* value)       {section_config_json[U(key)] = json::value::string(U(value));setModuleConfig(section_config, section_config_json);}

int         AppConfig::getInt(const char * key)    const {if (!isSectionEmpty() && section_config_json.has_field(key) ){ json::value v1 = section_config_json.at(U(key)); if ( v1.is_number())  {return v1.as_integer();} } return -1; } // i = v1.as_integer();
double      AppConfig::getDouble(const char * key) const {if (!isSectionEmpty() && section_config_json.has_field(key) ){ json::value v1 = section_config_json.at(U(key)); if ( v1.is_number())  {return v1.as_double();}  } return static_cast<double>(-1); } // d = v2.as_double();
bool        AppConfig::getBool(const char * key)   const {if (!isSectionEmpty() && section_config_json.has_field(key) ){ json::value v1 = section_config_json.at(U(key)); if ( v1.is_boolean()) {return v1.as_bool();}    } return false; } // b = v3.as_bool();
std::string AppConfig::getString(const char * key) const {if (!isSectionEmpty() && section_config_json.has_field(key) ){ json::value v1 = section_config_json.at(U(key)); if ( v1.is_string())  {return v1.as_string();}  } return ""; } // b = v3.as_bool();


void        AppConfig::setConfigLine( const char * key, const void * value) {
	for (const field_type field : field_array) {
		if (strcmp(key,field.key)==0) {
			switch (field.type) {
				case BOOLEAN:
					setBool(field.key, *static_cast<const bool*>(value));
					break;
				case INTEGER:
					setInt(field.key, *static_cast<const int*>(value));
					break;
				case DOUBLLE:
					setDouble(field.key, *static_cast<const double*>(value));
					break;
				case STRING:
					setString(field.key, static_cast<const char*>(value));
					break;
			}
		}
	}
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

