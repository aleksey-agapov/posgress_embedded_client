/*
 * AppConfigControl.h
 *
 *  Created on: Jan 27, 2021
 *      Author: user
 */

#ifndef CONFIG_APPCONFIGCONTROL_H_
#define CONFIG_APPCONFIGCONTROL_H_

#define DEFAULT_CONFIG "config.json"


#include <string.h>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <cpprest/json.h>                       // JSON library

using namespace web; //::json;                                  // JSON library




namespace config {

#define DEFAULT_CONFIG_NAME  "pg-sql.json"

enum ClassType {
	BOOLEAN = 0,
	INTEGER,
	DOUBLLE,
	STRING
};


struct field_type {
	const char * key;
	ClassType type;
	const void * default_val;
};


class IConfigCallBack {   // : public std::enable_shared_from_this<config::IConfigCallBack>
protected:
	virtual ~IConfigCallBack() = default ;

public:
	virtual void Update() {std::cerr << "IConfigCallBack->Update() is virtual Metod!!!" << std::endl;}
	IConfigCallBack& operator=(const IConfigCallBack&) = delete; // запрет присваивания

};


class AppConfigControl {
	private:
		std::string file_config;
		inline static json::value base_config;
		std::map<const char *, std::shared_ptr<IConfigCallBack> > callbackConnection;
	protected:
		json::value getSectionConfig(const char * section_name);
		void setModuleConfig(const char * section_name, json::value& section_value);
		void SaveDefault();
		bool isEmpty() const {return (base_config.size() == 0);}

		void UnRegisterCallback(const char * key );
		void Update() ; //{std::cerr << "AppConfigControl->Update() is virtual Metod!!!" << std::endl;}
	public:
		void RegisterCallback(const char * key, std::shared_ptr<IConfigCallBack> callback);
		AppConfigControl():AppConfigControl(DEFAULT_CONFIG){}
		AppConfigControl(std::string file_config);
		virtual ~AppConfigControl() = default;

		bool isContainModule (const char * KeyName) const;
		void LoadNewConfig(const char * FileName);
		void SaveConfig   (const char * FileName);
		void clear() {base_config= json::value::null();}
		std::unique_ptr<std::string> showTree(const char * config_module = nullptr);
};

// dbname = messagedb user = postgres password = 12345678 hostaddr = 127.0.0.1 port = 5432";


class AppConfig: public config::AppConfigControl {
	public:
		AppConfig(const char * section, const field_type int_field_array[], ssize_t field_size ) : section_config(section) {
			refreshConfig();
			for (ssize_t count= 0; count < field_size; count++) {
				const field_type field = int_field_array[count];
				field_array.push_back(field);
			}
		}
		virtual ~AppConfig() = default;

		void setConfigLine( const char * key, const void * value);

		bool        isSectionEmpty()            const;
		bool        isContainKey (const char * KeyName) const;
		int         getInt(const char * key)    const; // i = v1.as_integer();
		double      getDouble(const char * key) const; // d = v2.as_double();
		bool        getBool(const char * key)   const; // b = v3.as_bool();
		std::string getString(const char * key) const; // b = v3.as_bool();

		void        saveSession();

		const char * getModuleName() const {return section_config;}
		const std::vector<field_type>& getModule() const {return field_array;}

	protected:
		void        refreshConfig(){section_config_json = getSectionConfig(section_config); }
		void        setBool(const char * key, const bool value);
		void        setDouble(const char * key, const double value);
		void        setInt(const char * key, const int value);
		void        setString(const char * key, const char* value);

	private:
		const char * section_config;
		json::value section_config_json;
		std::vector<field_type> field_array;
};



class DbAppConfig final: public AppConfig, public config::IConfigCallBack {
	private:
		inline static const char *moduleName = "db";

		inline static const field_type int_field_array[] = { { "dbname", STRING,
				static_cast<const void*>("unknown") }, { "user", STRING,
				static_cast<const void*>("postgres") }, { "password", STRING,
				static_cast<const void*>("password") }, { "hostaddr", STRING,
				static_cast<const void*>("127.0.0.1") }, { "port", INTEGER,
				static_cast<const void*>((int[]) { 5432 } ) } };

	public:
		DbAppConfig() :
				AppConfig(moduleName, int_field_array,
						sizeof(int_field_array) / sizeof(int_field_array[0])) {
			if (this->isSectionEmpty()) {
				for (field_type field : int_field_array) {
					setConfigLine(field.key, field.default_val);
				}
				saveSession();
			}
		}
		virtual ~DbAppConfig() = default;

		std::string getConfigLine() {
			std::stringstream m_stream;
			refreshConfig();
			for (field_type field : int_field_array) {
				switch (field.type) {
				case BOOLEAN:
					m_stream << " " << field.key << " = " << getBool(field.key);
					break;
				case INTEGER:
					m_stream << " " << field.key << " = " << getInt(field.key);
					break;
				case DOUBLLE:
					m_stream << " " << field.key << " = "
							<< this->getDouble(field.key);
					break;
				case STRING:
					m_stream << " " << field.key << " = "
							<< this->getString(field.key);
					break;
				}
			}
			return m_stream.str();
		}
};
} /* namespace config */

#endif /* CONFIG_APPCONFIGCONTROL_H_ */
