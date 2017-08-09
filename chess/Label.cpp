#include "Label.h"

#include "Core.h"
#include "GUI.h"

#ifdef _WIN32
#define LOCALE "Russian"
#elif defined(__linux__)
#define LOCALE "ru_RU.CP1251"
#else
#error I don't know the encoding of this OS.
#endif

Label::Label(const std::string & text) :
    m_font(nullptr), m_wordwrap(false), m_alignment(GUI::AlignCenter), m_padding(0.0f)
{
	if (m_font == nullptr) {
		m_font = AssetManager::get<sf::Font>("font_default");
		m_text_geometry.setFont(*m_font);
		m_text_geometry.setString(text);
	}

	set_text(text);
}

void Label::set_text(const std::string & text)
{
	set_text(sf::String(text, std::locale(LOCALE)).toWideString());
}

void Label::set_text(const std::wstring & text)
{
	std::wstring string;

	if (m_wordwrap) {
		string = make_wordwrapped_text(text);
    }
	else {
		string = text;
	}

	m_text_geometry.setString(string);
	sf::FloatRect textBounds = m_text_geometry.getLocalBounds();
	set_minimum_size(textBounds.width, textBounds.height);
	update();
}

void Label::set_font(sf::Font* font)
{
	m_font = font;
	m_text_geometry.setFont(*font);
	update();
}

void Label::set_alignment(int alignment)
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

void Label::set_wordwrap_enabled(bool enabled)
{
	m_wordwrap = enabled;
	set_text(m_text_geometry.getString().toWideString());
	update();
}

void Label::set_padding(float padding)
{
	m_padding = padding;
	update();
}

void Label::set_color(const sf::Color & color)
{
	m_text_geometry.setFillColor(color);
}

void Label::set_text_outline_color(const sf::Color & color)
{
	m_text_geometry.setOutlineColor(color);
}

void Label::set_text_outline_thickness(float thickness)
{
	m_text_geometry.setOutlineThickness(thickness);
}

void Label::set_font_size(int size)
{
	m_text_geometry.setCharacterSize(size);
	sf::FloatRect textBounds = m_text_geometry.getLocalBounds();
	set_minimum_size(textBounds.width, textBounds.height);
	update();
}

void Label::set_font_style(unsigned int style)
{
	m_text_geometry.setStyle(style);
}

char Label::get_type() const
{
	return GUI::WidgetType::LabelWidget;
}

void Label::on_draw()
{
	if (m_is_visible) {
		Core::get_window()->draw(m_geometry);
		Core::get_window()->draw(m_text_geometry);
	}
}

void Label::update()
{
	sf::FloatRect textBounds = m_text_geometry.getLocalBounds();
	sf::FloatRect bounds = m_geometry.getGlobalBounds();
	sf::Vector2f position = sf::Vector2f(-textBounds.left, m_text_geometry.getCharacterSize() * -0.1875f);

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

	position.x = ceilf(position.x);
	position.y = ceilf(position.y);
	m_text_geometry.setPosition(position);
}

sf::String Label::make_wordwrapped_text(sf::String text)
{
	unsigned currentOffset = 0;
	bool firstWord = true;
	std::size_t wordBegining = 0;

	for (std::size_t pos = 0; pos < text.getSize(); ++pos) {
		auto currentChar = text[pos];
		if (currentChar == '\n') {
			currentOffset = 0;
			firstWord = true;
			continue;
		}
		else if (currentChar == ' ') {
			wordBegining = pos;
			firstWord = false;
		}

		sf::Glyph glyph = m_font->getGlyph(currentChar, m_text_geometry.getCharacterSize(), m_text_geometry.getStyle() & sf::Text::Bold);
		currentOffset += glyph.advance;

		if (!firstWord && currentOffset > m_geometry.getSize().x - m_padding * 2) {
			pos = wordBegining;
			text[pos] = '\n';
			firstWord = true;
			currentOffset = 0;
		}
	}

	return text;
}
