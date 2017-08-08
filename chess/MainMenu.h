#pragma once

#include <SFML/Graphics.hpp>

#include "State.h"
#include "GUI.h"

class MainMenu : public State
{
public:
	MainMenu();
	~MainMenu();

	void init() override;
	void resized(float width, float height) override;

	void update(const float dt) override;
	void draw(const float dt) override;
private:
	sf::Sprite m_background_sprite;

	std::shared_ptr<Label> create_label(const std::wstring& text);
	std::shared_ptr<Label> create_button(const std::wstring& text);
	std::shared_ptr<TextBox> create_textbox(bool masked);

	sf::Sprite m_logo;
	std::shared_ptr<Label> m_label_form;

	std::shared_ptr<Label> m_label_login;
	std::shared_ptr<TextBox> m_input_login;

	std::shared_ptr<Label> m_label_password;
	std::shared_ptr<TextBox> m_input_password;

	std::shared_ptr<Label> m_label_password_repeat;
	std::shared_ptr<TextBox> m_input_password_repeat;

	std::shared_ptr<Label> m_button_login;
	std::shared_ptr<Label> m_button_register;

	enum class MenuState {
		Authorization,
		Registration
	} m_state;
};