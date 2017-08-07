#pragma once

#include <memory>

#include <SFML/Graphics/Texture.hpp>

#include "Asset.h"

class TextureAsset : public Asset
{
public:
	void* getData() override
	{
		return &m_texture;
	}

private:
	friend class TextureFactory;

	sf::Texture m_texture;
};

class TextureFactory : public AssetFactory
{
public:
	TextureFactory(const std::string& path);
	~TextureFactory();

	Asset* load() override;
	void clear() override;

private:
	std::string m_path;
	std::shared_ptr<TextureAsset> m_asset;
};