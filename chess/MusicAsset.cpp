#include "MusicAsset.h"

MusicFactory::MusicFactory(const std::string & path) :
	m_path(path), m_asset(nullptr)
{
}

MusicFactory::~MusicFactory()
{
	if (m_asset != nullptr) {
		Log::write("music deleted: \"" + m_path + "\"");
	}
}

Asset * MusicFactory::load()
{
	if (m_asset == nullptr) {
		std::shared_ptr<MusicAsset> asset = std::make_shared<MusicAsset>();
		if (asset->m_music.openFromFile(m_path)) {
			m_asset = std::move(asset);
			Log::write("music loaded: \"" + m_path + "\"");
		}
		else {
			throw std::runtime_error("unable to load music: \"" + m_path + "\"");
		}
	}

	return m_asset.get();
}

void MusicFactory::clear()
{
	if (m_asset != nullptr) {
		Log::write("music deleted: \"" + m_path + "\"");
	}
	m_asset.reset();
}
