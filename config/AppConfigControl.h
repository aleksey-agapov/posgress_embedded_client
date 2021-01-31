/*
 * AppConfigControl.h
 *
 *  Created on: Jan 27, 2021
 *      Author: user
 */

#ifndef CONFIG_APPCONFIGCONTROL_H_
#define CONFIG_APPCONFIGCONTROL_H_

#define DEFAULT_CONFIG "config.json"



#include <string>
#include <memory>
#include <cpprest/json.h>                       // JSON library

using namespace web; //::json;                                  // JSON library




namespace config {

class IConfigCallBack {
public:
	virtual void Update() = 0;
	virtual ~IConfigCallBack() {}
};


class AppConfigControl {
private:
	std::string file_config;
	inline static json::value base_config;
	void LoadDefaultConfig();
	void SeveDefaultConfig();
protected:
	json::value getSectionConfig(const char * section_name);
public:
	AppConfigControl():AppConfigControl(DEFAULT_CONFIG){}
	AppConfigControl(std::string file_config);
	virtual ~AppConfigControl();

	void RegisterCallback(const char * key, std::shared_ptr<IConfigCallBack> callback);
	void UnRegisterCallback(const char * key );

	void LoadNewConfig(const std::string FileName);
	void SaveConfig   (const std::string FileName);

};

// dbname = messagedb user = postgres password = 12345678 hostaddr = 127.0.0.1 port = 5432";
class AppConfig: public config::AppConfigControl {
public:
	AppConfig(std::string section) : section_config(section) {
		section_config_json = getSectionConfig(section.c_str());
	}
	virtual ~AppConfig(){}

protected:

	bool        setFloat(const char * key, float value);
	bool        setBool(const char * key, bool value);
	bool        setDouble(const char * key, double value);
	bool        setInt(const char * key, int value);
	bool        setString(const char * key, const char* value);

	float       getFloat(const char * key);
	int         getInt(const char * key); // i = v1.as_integer();
	double      getDouble(const char * key); // d = v2.as_double();
	bool        getBool(const char * key); // b = v3.as_bool();
	std::string getString(const char * key); // b = v3.as_bool();


private:
	std::string section_config;
	json::value section_config_json;
};

class DbAppConfig: public  AppConfig{
private:
	inline static const char DB_CONFIG_TEMPLATE = "dbname = %s user = %s password = %s hostaddr = %s port = %d";

public:
DbAppConfig() : AppConfig("db"){}
virtual ~DbAppConfig() {}


void setConfigLine() {

}



std::string getConfigLine() {
	std::stringstream m_stream;
	m_stream << "dbname = " << getString("dbname") << " user = " << getString("user") << " password = " << getString("password") << " hostaddr = " << getString("hostaddr") << " port = " << getInt("port");

/*
	char string_buff[512];
	sprintf(string_buff, DB_CONFIG_TEMPLATE,
			getString("dbname").c_str(),
			getString("user").c_str(),
			getString("password").c_str(),
			getString("hostaddr").c_str(),
			getInt("port"));
	std::string ret(string_buff);
	*/
	return m_stream.str();
}
};
} /* namespace config */

#endif /* CONFIG_APPCONFIGCONTROL_H_ */
