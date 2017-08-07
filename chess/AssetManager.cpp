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

	std::string dataFolder = path;
	if (dataFolder.find_last_of('/') > 0) {
		dataFolder = path.substr(0, path.find_last_of('/'));
	}
	dataFolder += "/";

	auto audioData = data["audio"];
	if (audioData.is_object()) {
		std::string resourceFolder = "audio/";
		for (auto it = audioData.begin(); it != audioData.end(); ++it) {
			const json& value = it.value();
			if (value.is_string()) {
				AssetManager::bind<MusicFactory>(it.key(), dataFolder + resourceFolder + value.get<std::string>());
			}
		}
	}

	auto fontsData = data["fonts"];
	if (fontsData.is_object()) {
		std::string resourceFolder = "fonts/";
		for (auto it = fontsData.begin(); it != fontsData.end(); ++it) {
			const json& value = it.value();
			if (value.is_string()) {
				AssetManager::bind<FontFactory>(it.key(), dataFolder + resourceFolder + value.get<std::string>());
			}
		}
	}

	auto shadersData = data["shaders"];
	if (shadersData.is_object()) {
		std::string resourceFolder = "shaders/";
		for (auto it = shadersData.begin(); it != shadersData.end(); ++it) {
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
				AssetManager::bind<ShaderFactory>(it.key(), dataFolder + resourceFolder + value.get<std::string>(), shaderType);
			}
		}
	}

	auto texturesData = data["textures"];
	if (texturesData.is_object()) {
		std::string resourceFolder = "textures/";
		for (auto it = texturesData.begin(); it != texturesData.end(); ++it) {
			const json& value = it.value();
			if (value.is_string()) {
				AssetManager::bind<TextureFactory>(it.key(), dataFolder + resourceFolder + value.get<std::string>());
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
