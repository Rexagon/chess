#include "FontAsset.h"

FontFactory::FontFactory(const std::string & path) :
	m_path(path), m_asset(nullptr)
{
}

FontFactory::~FontFactory()
{
	if (m_asset != nullptr) {
		Log::write("font deleted: \"" + m_path + "\"");
	}
}

Asset * FontFactory::load()
{
	if (m_asset == nullptr) {
		std::shared_ptr<FontAsset> asset = std::make_shared<FontAsset>();
		if (asset->m_font.loadFromFile(m_path)) {
			m_asset = std::move(asset);
			Log::write("font loaded: \"" + m_path + "\"");
		}
		else {
			throw std::runtime_error("unable to load fonr: \"" + m_path + "\"");
		}
	}

	return m_asset.get();
}

void FontFactory::clear()
{
	if (m_asset != nullptr) {
		Log::write("font deleted: \"" + m_path + "\"");
	}
	m_asset.reset();
}
