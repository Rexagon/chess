#pragma once

#include <json.hpp>

using json = nlohmann::json;

class ConfigManager
{
public:
	static void init();
	static void close();

	template <class T>
	void add(const std::string& name, const T& value)
	{
		m_config[name] = value;
	}

	static void add(const std::string& name, const std::wstring& value);

	static bool has(const std::string& name);

	template <typename T>
	static typename std::enable_if<std::is_same<T, std::wstring>::value, T>::type 
		get(const std::string& name) 
	{
		if (has(name)) {
			std::vector<char> data = m_config[name];

			std::wstring result;
			result.resize(data.size() / 2);

			for (unsigned int i = 0; i < data.size(); ++i) {
				reinterpret_cast<char*>(&result[0])[i] = data[i];
			}

			return result;
		}
		else {
			return std::wstring();
		}
	}

	template <typename T>
	static typename std::enable_if<!std::is_same<T, std::wstring>::value, T>::type
		get(const std::string& name)
	{
		if (has(name)) {
			return m_config[name].get<T>();
		}
		else {
			return T();
		}
	}
private:
	static json m_config;
};