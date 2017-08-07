#pragma once

#include <memory>

#include <SFML/Graphics/Font.hpp>

#include "Asset.h"

class FontAsset : public Asset
{
public:
	void * getData() override {
		return &m_font;
	}

private:
	friend class FontFactory;

	sf::Font m_font;
};

class FontFactory : public AssetFactory
{
public:
	FontFactory(const std::string& path);
	~FontFactory();

	Asset* load() override;
	void clear() override;

private:
	std::string m_path;
	std::shared_ptr<FontAsset> m_asset;
};