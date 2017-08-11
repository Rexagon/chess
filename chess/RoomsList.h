#pragma once

#include <mutex>

#include "State.h"
#include "GUI.h"

class RoomsList : public State
{
public:
	RoomsList();
	~RoomsList();

	void init() override;

	void update(const float dt) override;
	void draw(const float dt) override;

	void resized(float width, float height) override;
private:
	sf::Sprite m_background_sprite;

	void update_rooms_list();

	std::shared_ptr<Label> create_label(const std::wstring& text);
	std::shared_ptr<Label> create_button(const std::wstring& text);
	std::shared_ptr<TextBox> create_textbox();

	std::shared_ptr<Label> m_form_list;

	std::shared_ptr<Widget> m_widget_list;

	unsigned int m_current_page;

	struct Room
	{
		std::wstring name;
		std::wstring white_player;
		std::wstring black_player;
		int spectators_count;
	};

	std::vector<Room> m_rooms;
	std::mutex m_rooms_mutex;
	std::vector<std::shared_ptr<Widget>> m_rooms_widgets;
};