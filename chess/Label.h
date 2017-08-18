#pragma once

#include "Widget.h"

class Label : public Widget
{
public:
	Label(const std::string& text = std::string());

	virtual void set_text(const std::string& text);
	virtual void set_text(const std::wstring& text);
	virtual sf::String get_text() const { return m_text_geometry.getString(); }

	void set_font(sf::Font* font);
	sf::Font* get_font() const { return m_font; }

	void set_alignment(int alignment);
	int get_alignment() const { return m_alignment; }

	void set_wordwrap_enabled(bool enabled);
	bool get_wordwrap_enabled() const { return m_wordwrap; }

	void set_padding(float padding);
	float get_padding() const { return m_padding; }

	void set_color(const sf::Color& color);
	sf::Color get_color() const { return m_text_geometry.getFillColor(); }

	void set_text_outline_color(const sf::Color &color);
	sf::Color get_text_outline_color() const { return m_text_geometry.getOutlineColor(); }

	void set_text_outline_thickness(float thickness);
	float get_text_outline_thickness() const { return m_text_geometry.getOutlineThickness(); }

	void set_font_size(int size);
	int get_font_size() const { return m_text_geometry.getCharacterSize(); }

	void set_font_style(unsigned int style);
	unsigned int get_font_style() const { return m_text_geometry.getStyle(); }

	virtual char get_type() const;
protected:
	void on_draw() override;

	// Обновляет геометрию
	void update_geometry() override;

	sf::String make_wordwrapped_text(sf::String text);

	sf::Font* m_font;
	int m_alignment;
	bool m_wordwrap;
	float m_padding;

	sf::Font temp;

	sf::Text m_text_geometry;
};