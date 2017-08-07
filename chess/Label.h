#pragma once

#include "Widget.h"

class Label : public Widget
{
public:
	Label(const std::string& text = std::string(), sf::Font* font = nullptr);

	void setText(const std::string& text);
	void setText(const std::wstring& text);
	std::string GetText() const { return m_geometryText.getString(); }

	void setFont(sf::Font* font);
	sf::Font* getFont() const { return m_font; }

	void setAlignment(int alignment);
	int getAlignment() const { return m_alignment; }

	void setWordwrapEnabled(bool enabled);
	bool getWordwrapEnabled() const { return m_wordwrap; }

	void setPadding(float padding);
	float getPadding() const { return m_padding; }

	void setColor(const sf::Color& color);
	sf::Color getColor() const { return m_geometryText.getFillColor(); }

	void setOutlineColor(const sf::Color &color);
	sf::Color getOutlineColor() const { return m_geometryText.getOutlineColor(); }

	void setOutlineThickness(float thickness);
	float getOutlineThickness() const { return m_geometryText.getOutlineThickness(); }

	void setFontSize(int size);
	int getFontSize() const { return m_geometryText.getCharacterSize(); }

	void setFontStyle(unsigned int style);
	unsigned int getFontStyle() const { return m_geometryText.getStyle(); }
protected:
	void onDraw() override;

	// Обновляет геометрию
	void update() override;

	sf::Font* m_font;
	int m_alignment;
	bool m_wordwrap;
	float m_padding;

	sf::Font temp;

	sf::Text m_geometryText;
};