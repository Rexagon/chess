#pragma once

#include <memory>

#include <SFML/Graphics/Shader.hpp>

#include "Asset.h"

class ShaderAsset : public Asset
{
public:
	void * get_data() override
	{
		return &m_shader;
	}

private:
	friend class ShaderFactory;

	sf::Shader m_shader;
};

class ShaderFactory : public AssetFactory
{
public:
	ShaderFactory(const std::string& shader_path, sf::Shader::Type shader_type);
	ShaderFactory(const std::string& vertex_shader_path, const std::string& fragment_shader_path);
	~ShaderFactory();

	Asset* load() override;
	void clear() override;

private:
	std::string m_vertex_shader_path;
	std::string m_fragment_shader_path;
	std::shared_ptr<ShaderAsset> m_asset;
};