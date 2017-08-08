#include "TextBox.h"

#include "Core.h"
#include "GUI.h"

#ifdef _WIN32
#define LOCALE "Russian"
#elif defined(__linux__)
#define LOCALE "ru_RU.CP1251"
#else
#error I don't know the encoding of this OS.
#endif

TextBox::TextBox(const std::string & text) :
	Label(text), m_show_caret(false), m_max_text_length(0), m_timer(0.0f),
	m_masking_character('*'), m_is_masking_character_enabled(false)
{
	m_caret_position = text.size();
}

void TextBox::set_text(const std::string & text)
{
	set_text(sf::String(text, std::locale(LOCALE)).toWideString());
}

void TextBox::set_text(const std::wstring & text)
{
	if (m_wordwrap) {
		m_current_text = make_wordwrapped_text(text);
	}
	else {
		m_current_text = text;
	}

	if (m_is_masking_character_enabled) {
		sf::String masked_text = std::wstring(m_current_text.getSize(), m_masking_character);
		m_text_geometry.setString(masked_text);
	}
	else {
		m_text_geometry.setString(m_current_text);
	}

	sf::FloatRect textBounds = m_text_geometry.getLocalBounds();
	set_minimum_size(textBounds.width, textBounds.height);
	update();
	
	m_caret_position = text.size();
}

void TextBox::set_max_text_length(unsigned int size)
{
	m_max_text_length = size;
	sf::String current_text = m_text_geometry.getString();
	unsigned int current_text_size = current_text.getSize();
	if (current_text_size > m_max_text_length) {
		int diff = m_max_text_length - current_text_size;
		current_text.erase(m_max_text_length, diff);
	}
}

void TextBox::set_masking_character_enabled(bool enabled)
{
	m_is_masking_character_enabled = enabled;
	set_text(m_current_text.toWideString());
}

void TextBox::set_masking_character(sf::Uint32 character)
{
	m_masking_character = character;
	set_text(m_current_text.toWideString());
}

void TextBox::handle_text_enter(sf::Uint32 text)
{
	sf::String current_text = m_text_geometry.getString();
	int new_caret_position = m_caret_position;

	if (text == 8) { // backspace
		if (m_caret_position > 0) {
			new_caret_position--;
			current_text.erase(new_caret_position, text);
		}
	}
	else if (text == 1) { // left

	}
	else if (text == 3) { // right

	}
	else if (text == 13) {

	}
	else if (m_max_text_length == 0 || current_text.getSize() < m_max_text_length) {
		current_text.insert(new_caret_position, text);
		new_caret_position++;
	}

	set_text(current_text.toWideString());
	m_caret_position = new_caret_position;
}

char TextBox::get_type() const
{
	return GUI::WidgetType::TextBoxWidget;
}

void TextBox::on_update(const float dt)
{
	m_timer += dt;
	if (m_timer > 0.5f) {
		m_timer = 0.0f;
		m_show_caret = !m_show_caret;
	}
}

void TextBox::on_draw()
{
	if (m_is_visible) {
		Core::get_window()->draw(m_geometry);

		sf::String current_text = m_text_geometry.getString();

		if (m_is_focused) {
			sf::String current_text = m_text_geometry.getString();

			if (m_show_caret) {
				m_text_geometry.setString(current_text + "_");
			}

			Core::get_window()->draw(m_text_geometry);

			m_text_geometry.setString(current_text);
		}
		else {
			Core::get_window()->draw(m_text_geometry);
		}
	}
}
