#include "TextureAsset.h"

TextureFactory::TextureFactory(const std::string & path) :
	m_path(path), m_asset(nullptr)
{
}

TextureFactory::~TextureFactory()
{
	if (m_asset != nullptr) {
		Log::write("texture deleted: \"" + m_path + "\"");
	}
}

Asset * TextureFactory::load()
{
	if (m_asset == nullptr) {
		std::shared_ptr<TextureAsset> asset = std::make_shared<TextureAsset>();
		if (asset->m_texture.loadFromFile(m_path)) {
			m_asset = std::move(asset);
			Log::write("texture loaded: \"" + m_path + "\"");
		}
		else {
			throw std::runtime_error("unable to load texture: \"" + m_path + "\"");
		}
	}

	return m_asset.get();
}

void TextureFactory::clear()
{
	if (m_asset != nullptr) {
		Log::write("texture deleted: \"" + m_path + "\"");
	}
	m_asset.reset();
}
