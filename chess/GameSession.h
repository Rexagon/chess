#pragma once

#include "State.h"
#include "GUI.h"

class GameSession : public State
{
public:
	GameSession(const std::wstring& name);
	~GameSession();

	void init() override;

	void update(const float dt) override;
	void draw(const float dt) override;

	void resized(float width, float height) override;
private:
	sf::Sprite m_background_sprite;

	sf::Sprite m_board_sprite;

	std::vector<sf::Sprite> m_figure_sprites;

	std::shared_ptr<Widget> m_widget_board;
	std::vector<std::shared_ptr<Widget>> m_cell_widgets;

	std::wstring m_room_name;
};