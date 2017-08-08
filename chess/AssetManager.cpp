#include "AssetManager.h"

#include <fstream>

#include <json.hpp>

using json = nlohmann::json;

std::map<std::string, std::unique_ptr<AssetFactory>> AssetManager::m_factories;

void AssetManager::load(const std::string & path)
{
	std::ifstream file(path);
	if (!file.is_open()) {
		throw std::runtime_error("unable to load assets structure from: \"" + path + "\"");
	}

	json data;
	file >> data;

	std::string data_folder = path;
	if (data_folder.find_last_of('/') > 0) {
		data_folder = path.substr(0, path.find_last_of('/'));
	}
	data_folder += "/";

	auto audio_data = data["audio"];
	if (audio_data.is_object()) {
		std::string resource_folder = "audio/";
		for (auto it = audio_data.begin(); it != audio_data.end(); ++it) {
			const json& value = it.value();
			if (value.is_string()) {
				AssetManager::bind<MusicFactory>(it.key(), data_folder + resource_folder + value.get<std::string>());
			}
		}
	}

	auto fonts_data = data["fonts"];
	if (fonts_data.is_object()) {
		std::string resource_folder = "fonts/";
		for (auto it = fonts_data.begin(); it != fonts_data.end(); ++it) {
			const json& value = it.value();
			if (value.is_string()) {
				AssetManager::bind<FontFactory>(it.key(), data_folder + resource_folder + value.get<std::string>());
			}
		}
	}

	auto shaders_data = data["shaders"];
	if (shaders_data.is_object()) {
		std::string resource_folder = "shaders/";
		for (auto it = shaders_data.begin(); it != shaders_data.end(); ++it) {
			const json& value = it.value();
			if (value.is_string()) {
				std::string extension = value.get<std::string>();
				extension = extension.substr(extension.find_last_of('.'));
				sf::Shader::Type shaderType;
				if (extension == ".frag") {
					shaderType = sf::Shader::Fragment;
				}
				else {
					shaderType = sf::Shader::Vertex;
				}
				AssetManager::bind<ShaderFactory>(it.key(), data_folder + resource_folder + value.get<std::string>(), shaderType);
			}
		}
	}

	auto textures_data = data["textures"];
	if (textures_data.is_object()) {
		std::string resource_folder = "textures/";
		for (auto it = textures_data.begin(); it != textures_data.end(); ++it) {
			const json& value = it.value();
			if (value.is_string()) {
				AssetManager::bind<TextureFactory>(it.key(), data_folder + resource_folder + value.get<std::string>());
			}
		}
	}
}

void AssetManager::unbind(const std::string & name)
{
	auto it = m_factories.find(name);
	if (it != m_factories.end()) {
		m_factories.erase(it);
	}
}

void AssetManager::clear(const std::string & name)
{
	auto it = m_factories.find(name);
	if (it != m_factories.end()) {
		it->second->clear();
	}
}

void AssetManager::clear()
{
	m_factories.clear();
}
