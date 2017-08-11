#include "ConfigManager.h"

#include <fstream>
#include <vector>

json ConfigManager::m_config;

void ConfigManager::init()
{
	std::ifstream file("config", std::ios::binary);
	if (file.is_open()) {
		std::vector<uint8_t> input_vector((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		encrypt_decrypt(input_vector);
		m_config = json::from_cbor(input_vector);
	}
}

void ConfigManager::close()
{
	std::ofstream file("config", std::ios::out | std::ios::binary);
	if (file.is_open()) {
		std::vector<uint8_t> output_vector = json::to_cbor(m_config);
		encrypt_decrypt(output_vector);
		file.write(reinterpret_cast<char*>(output_vector.data()), output_vector.size());
	}
}

void ConfigManager::add(const std::string & name, const std::wstring & value)
{
	std::vector<char> data(value.size() * 2);
	for (unsigned int i = 0; i < value.size() * 2; ++i) {
		data[i] = reinterpret_cast<const char*>(&value[0])[i];
	}

	m_config[name] = data;
}

bool ConfigManager::has(const std::string & name)
{
	return m_config.find(name) != m_config.end();
}

void ConfigManager::encrypt_decrypt(std::vector<uint8_t>& data)
{
	char key[5] = { 'e', 'B', 'a', 'L', '!' };

	for (unsigned int i = 0; i < data.size(); i++) {
		data[i] ^= key[i % (sizeof(key) / sizeof(char))];
	}
}
