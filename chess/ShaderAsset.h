#pragma once

#include <memory>

#include <SFML/Graphics/Shader.hpp>

#include "Asset.h"

class ShaderAsset : public Asset
{
public:
	void * getData() override
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
	ShaderFactory(const std::string& shaderPath, sf::Shader::Type shaderType);
	ShaderFactory(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	~ShaderFactory();

	Asset* load() override;
	void clear() override;

private:
	std::string m_vertexShaderPath;
	std::string m_fragmentShaderPath;
	std::shared_ptr<ShaderAsset> m_asset;
};