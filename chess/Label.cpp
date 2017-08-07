#include "Label.h"

#include "Core.h"
#include "GUI.h"

#ifdef __linux__
#define LOCALE "ru_RU.CP1251"
#else
#define LOCALE "Russian"
#endif

Label::Label(const std::string & text, sf::Font* font) :
    m_font(font), m_wordwrap(false), m_alignment(GUI::AlignCenter), m_padding(0.0f)
{
	if (m_font == nullptr) {
		m_font = AssetManager::get<sf::Font>("font_default");
		m_geometryText.setFont(*m_font);
		m_geometryText.setString(text);
	}

	setText(text);
}

void Label::setText(const std::string & text)
{
	std::string string = text;

	if (m_wordwrap) {
		unsigned currentOffset = 0;
		bool firstWord = true;
		std::size_t wordBegining = 0;

        for (std::size_t pos = 0; pos < string.size(); ++pos) {
			auto currentChar = string[pos];
			if (currentChar == '\n') {
				currentOffset = 0;
				firstWord = true;
				continue;
			}
			else if (currentChar == ' ') {
				wordBegining = pos;
				firstWord = false;
			}

			sf::Glyph glyph = m_font->getGlyph(currentChar, m_geometryText.getCharacterSize(), m_geometryText.getStyle() & sf::Text::Bold);
			currentOffset += glyph.advance;

			if (!firstWord && currentOffset > m_geometry.getSize().x) {
				pos = wordBegining;
				string[pos] = '\n';
				firstWord = true;
				currentOffset = 0;
			}
		}
    }

    m_geometryText.setString(sf::String(string, std::locale(LOCALE)));
	sf::FloatRect textBounds = m_geometryText.getLocalBounds();
	setMinimumSize(textBounds.width, textBounds.height);
	update();
}

void Label::setText(const std::wstring & text)
{
	std::wstring string = text;

	if (m_wordwrap) {
		unsigned currentOffset = 0;
		bool firstWord = true;
		std::size_t wordBegining = 0;

		for (std::size_t pos(0); pos < string.size(); ++pos) {
			auto currentChar = string[pos];
			if (currentChar == '\n') {
				currentOffset = 0;
				firstWord = true;
				continue;
			}
			else if (currentChar == ' ') {
				wordBegining = pos;
				firstWord = false;
			}

			sf::Glyph glyph = m_font->getGlyph(currentChar, m_geometryText.getCharacterSize(), m_geometryText.getStyle() & sf::Text::Bold);
			currentOffset += glyph.advance;

			if (!firstWord && currentOffset > m_geometry.getSize().x) {
				pos = wordBegining;
				string[pos] = '\n';
				firstWord = true;
				currentOffset = 0;
			}
		}
    }

	m_geometryText.setString(string);
	sf::FloatRect textBounds = m_geometryText.getLocalBounds();
	setMinimumSize(textBounds.width, textBounds.height);
	update();
}

void Label::setFont(sf::Font* font)
{
	m_font = font;
	m_geometryText.setFont(*font);
	update();
}

void Label::setAlignment(int alignment)
{
	if (!(alignment & GUI::AlignLeft ||
		alignment & GUI::AlignRight ||
		alignment & GUI::AlignHCenter)) {
		alignment |= GUI::AlignHCenter;
	}

	if (!(alignment & GUI::AlignTop ||
		alignment & GUI::AlignBottom ||
		alignment & GUI::AlignVCenter)) {
		alignment |= GUI::AlignVCenter;
	}

	m_alignment = alignment;
	update();
}

void Label::setWordwrapEnabled(bool enabled)
{
	m_wordwrap = enabled;
	setText(m_geometryText.getString().toWideString());
	update();
}

void Label::setPadding(float padding)
{
	m_padding = padding;
	update();
}

void Label::setColor(const sf::Color & color)
{
	m_geometryText.setFillColor(color);
}

void Label::setOutlineColor(const sf::Color & color)
{
	m_geometryText.setOutlineColor(color);
}

void Label::setOutlineThickness(float thickness)
{
	m_geometryText.setOutlineThickness(thickness);
}

void Label::setFontSize(int size)
{
	m_geometryText.setCharacterSize(size);
	sf::FloatRect textBounds = m_geometryText.getLocalBounds();
	setMinimumSize(textBounds.width, textBounds.height);
	update();
}

void Label::setFontStyle(unsigned int style)
{
	m_geometryText.setStyle(style);
}

void Label::onDraw()
{
	if (m_isVisible) {
		Core::getWindow()->draw(m_geometry);
		Core::getWindow()->draw(m_geometryText);
	}
}

void Label::update()
{
	sf::FloatRect textBounds = m_geometryText.getLocalBounds();
	sf::FloatRect bounds = m_geometry.getGlobalBounds();
	sf::Vector2f position = sf::Vector2f(-textBounds.left, m_geometryText.getCharacterSize() * -0.1875f);

	if (m_alignment & GUI::AlignLeft) {
		position.x += bounds.left + m_padding;
	}
	else if (m_alignment & GUI::AlignRight) {
		position.x += bounds.left + bounds.width - m_padding - textBounds.width;
	}
	else if (m_alignment & GUI::AlignHCenter) {
		position.x += bounds.left + (bounds.width - textBounds.width) / 2.0f;
	}

	if (m_alignment & GUI::AlignTop) {
		position.y += bounds.top + m_padding;
	}
	else if (m_alignment & GUI::AlignBottom) {
		position.y += bounds.top + bounds.height - m_padding - textBounds.height;
	}
	else if (m_alignment & GUI::AlignVCenter) {
		position.y += bounds.top + (bounds.height - textBounds.height) / 2.0f;
	}

	m_geometryText.setPosition(position);
}
