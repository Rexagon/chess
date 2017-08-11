#pragma once

#include "State.h"
#include "GUI.h"

class MainMenu : public State
{
public:
	MainMenu();
	~MainMenu();

	void update(const float dt) override;
	void draw(const float dt) override;

	void scene_leave() override;
	void scene_return() override;

	void resized(float width, float height) override;
private:
	enum class MenuState {
		Authorization,
		ProcessiogAuthorization,
		Registration,
		PrecessingRegistration,
	} m_state;

	void update_menu_state(MenuState state);
	void show_error(const std::wstring& text);
	void show_info(const std::wstring& text);

	void make_authorization();
	void make_registration();

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
	std::shared_ptr<Label> m_button_exit;

	std::shared_ptr<Label> m_label_message;
};