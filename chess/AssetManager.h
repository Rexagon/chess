#pragma once

#include <functional>
#include <memory>
#include <string>
#include <map>

#include "Asset.h"
#include "TextureAsset.h"
#include "ShaderAsset.h"
#include "MusicAsset.h"
#include "FontAsset.h"

class AssetManager
{
public:
	// Загружает информацию о ресурсах из json файла
	/// 'узлы" структуры - папки
	/// 'листья' структуры - файлы
	/// пути файлов складываются из всех его предыдущих узлов
	/// все результирующие бути считаются относительно папки, в которой лежит json файл
	static void load(const std::string& path);

	// Привязывает фабрику ресурса к его уникальному названию
	template<class T, class... Args>
	static void bind(const std::string& name, Args&&... factory_arguments)
	{
		auto it = m_factories.find(name);
		if (it == m_factories.end()) {
			m_factories[name] = std::unique_ptr<T>(new T(std::forward<Args>(factory_arguments)...));
		}
	}

	// Отвязывает фабрику ресурса от его названия
	static void unbind(const std::string& name);

	// Возвращает ресурс по его уникальному названию
	//@ если такого нет, то nullptr
	template<class T>
	static T* get(const std::string& name) {
		auto it = m_factories.find(name);
		if (it == m_factories.end()) {
			Log::write("unable to find resource: \"" + name + "\"");
			return nullptr;
		}
		else {
			return reinterpret_cast<T*>(it->second->load()->get_data());
		}
	}

	static void clear(const std::string& name);

	static void clear();
private:
	static std::map<std::string, std::unique_ptr<AssetFactory>> m_factories;
};
