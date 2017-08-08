#pragma once

#include "Label.h"

class TextBox : public Label
{
public:
	TextBox(const std::string& text = std::string());

	virtual void set_text(const std::string& text);
	virtual void set_text(const std::wstring& text);
	virtual sf::String get_text() const { return m_current_text; }

	// Setting max text length
	//@ 0 is unlimited
	void set_max_text_length(unsigned int size);
	unsigned int get_max_text_length() const { return m_max_text_length; }

	void set_masking_character_enabled(bool enabled);
	bool is_masking_character_enabled() const { return m_is_masking_character_enabled; }

	void set_masking_character(sf::Uint32 character);
	sf::Uint32 get_masking_character() const { return m_masking_character; }

	void handle_text_enter(sf::Uint32 text);

	virtual char get_type() const;
private:
	void on_update(const float dt) override;

	void on_draw() override;

	unsigned int m_caret_position;
	bool m_show_caret;

	float m_timer;

	bool m_is_masking_character_enabled;
	sf::Uint32 m_masking_character;

	unsigned int m_max_text_length;

	sf::String m_current_text;
};