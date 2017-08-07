#pragma once

#include <memory>

#include <SFML/Audio/Music.hpp>

#include "Asset.h"

class MusicAsset : public Asset
{
public:
	void* getData() override
	{
		return &m_music;
	}

private:
	friend class MusicFactory;

	sf::Music m_music;
};

class MusicFactory : public AssetFactory
{
public:
	MusicFactory(const std::string& path);
	~MusicFactory();

	Asset* load() override;
	void clear() override;

private:
	std::string m_path;
	std::shared_ptr<MusicAsset> m_asset;
};