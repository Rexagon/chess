#pragma once

#include <mutex>

#include "State.h"
#include "Board.h"
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
	enum HighlightType {
		HighlightDisabled,
		HighlightSelected,
		HighlightPossible
	};

	enum PlayerType {
		Spectator,
		BlackPlayer,
		WhitePlayer
	};

	void init_board_gui();
	
	void set_cell_highlight(Widget* widget, HighlightType highlight_type);

	void make_move(const vec2c last_pos, const vec2c& next_pos);
	
	std::shared_ptr<Label> create_label(const std::wstring& text);
	std::shared_ptr<Label> create_button(const std::wstring& text);
	std::shared_ptr<TextBox> create_textbox(bool masked);

	Board m_board;
	
	sf::Sprite m_background_sprite;
	
	std::shared_ptr<Widget> m_widget_board;
	std::vector<std::shared_ptr<Widget>> m_cell_widgets;

	std::shared_ptr<Label> m_label_form;
	std::shared_ptr<Label> m_label_white_player;
	std::shared_ptr<Label> m_label_black_player;
	std::shared_ptr<Label> m_label_turn;

	vec2c m_selected_cell;
	Widget* m_selected_widget;
	std::vector<vec2c> m_possible_moves;

	std::recursive_mutex m_gui_mutex;
	
	PlayerType m_player_type;

	std::wstring m_room_name;
	std::wstring m_white_player;
	std::wstring m_black_player;
	
	bool m_is_private;
	bool m_is_chat_enabled;
	bool m_can_change_settings;
};