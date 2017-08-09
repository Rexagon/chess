#pragma once

#include "State.h"
#include "GUI.h"

class RoomsList : public State
{
public:
	RoomsList();
	~RoomsList();

	void update(const float dt) override;
	void draw(const float dt) override;

	void resized(float width, float height) override;
private:
	sf::Sprite m_background_sprite;

	std::shared_ptr<Label> create_label(const std::wstring& text);
	std::shared_ptr<Label> create_button(const std::wstring& text);
	std::shared_ptr<TextBox> create_textbox();

	std::shared_ptr<Label> m_form_list;
};