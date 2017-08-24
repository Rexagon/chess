#include "GameSession.h"

#include "Core.h"

GameSession::GameSession(const std::wstring& name) :
	m_room_name(name), m_cell_widgets(64), m_selected_cell(-1, -1), m_selected_widget(nullptr),
	m_player_type(Spectator)
{
	CursorManager::set_style(CursorManager::Normal);

	sf::Texture* background_texture = AssetManager::get<sf::Texture>("game_background");
	background_texture->setSmooth(true);
	m_background_sprite.setTexture(*background_texture, true);
	m_background_sprite.setOrigin(vec2(background_texture->getSize()) / 2.0f);
	
	// Create GUI
	//TODO: add chat and music here

	// Update layout
	vec2 window_size = vec2(Core::get_window()->getSize());
	resized(window_size.x, window_size.y);
}

GameSession::~GameSession()
{
	Client::remove_command_handler(CommandPacket::RoomSettings);
	Client::send_command(CommandPacket(CommandPacket::ChangeRoom, {}));
}

void GameSession::init()
{
	Client::add_command_handler(CommandPacket::RoomSettings, [this](const CommandPacket& packet) {
		if (packet.get_arguments_count() == 7) {
			std::vector<std::wstring> arguments = packet.get_arguments();
			m_room_name = arguments[1];
			
			m_white_player = arguments[3];
			m_black_player = arguments[4];

			if (Client::get_login() == m_white_player) {
				m_player_type = WhitePlayer;
				m_board.set_main_color(Figure::Color::White);
				m_board.init_field();
				init_board_gui();
			}
			else if (Client::get_login() == m_black_player) {
				m_player_type = BlackPlayer;
				m_board.set_main_color(Figure::Color::Black);
				m_board.init_field();
				init_board_gui();
			}
			else {
				m_player_type = Spectator;
				m_board.set_main_color(Figure::Color::White);
				m_board.init_field();
			}

			m_is_private = arguments[5] == L"1";
			m_is_chat_enabled = arguments[6] == L"1";
			m_can_change_settings = arguments[2] == Client::get_login();
		}
	}, true);

	Client::send_command(CommandPacket(CommandPacket::RoomSettings, { m_room_name }));
}

void GameSession::update(const float dt)
{
	m_gui.update(dt);

	if (Input::get_key_down(Key::Escape)) {
		Core::delete_state();
		return;
	}
}

void GameSession::draw(const float dt)
{
	Core::draw(m_background_sprite);

	m_board.draw_board(Core::get_window());

	m_gui.draw();

	m_board.draw_figures(Core::get_window());
}

void GameSession::resized(float width, float height)
{
	Core::get_window()->setView(sf::View(sf::FloatRect(0, 0, width, height)));

	vec2 background_scale(1.0f, 1.0f);
	vec2 background_size = vec2(m_background_sprite.getTexture()->getSize());

	m_background_sprite.setPosition(width / 2.0f, height / 2.0f);

	background_scale.x = width / background_size.x;
	background_scale.y = height / background_size.y;

	float scale = std::max(background_scale.x, background_scale.y);
	m_background_sprite.setScale(scale, scale);

	// Board layout
	vec2 board_size = m_board.get_size();
	vec2 board_position = vec2((width - board_size.x) / 2.0f, height / 2.0f);
	if (board_position.x < (board_size.x + 20.0f) / 2.0f) {
		board_position.x = (board_size.x + 20.0f) / 2.0f;
	}
	board_position.x = ceilf(board_position.x);
	board_position.y = ceilf(board_position.y);

	m_board.setPosition(board_position);

	if (m_player_type != Spectator && m_board.is_initialized() == true) {
		vec2 field_offset = board_position + vec2(54.0f, 54.0f) - board_size / 2.0f;
		field_offset.x = ceilf(field_offset.x);
		field_offset.y = ceilf(field_offset.y);

		for (unsigned int i = 0; i < 8; ++i) {
			for (unsigned int j = 0; j < 8; ++j) {
				vec2 cell_position;
				if (m_player_type == WhitePlayer) {
					cell_position = vec2(i * m_board.get_cell_size().x, (7 - j) * m_board.get_cell_size().y);
				}
				else {
					cell_position = vec2((7 - i) * m_board.get_cell_size().x, j * m_board.get_cell_size().y);
				}

				m_cell_widgets[i * 8 + j]->set_position(field_offset + cell_position);
			}
		}
	}
}

void GameSession::init_board_gui()
{
	m_widget_board = m_gui.create<Widget>();
	Layout* board_layout = new Layout(m_widget_board);

	auto hover_function = [this](Widget* widget, const vec2c& widget_coords) {
		sf::Color widget_color = sf::Color(100, 100, 100, 100);

		for (const auto& possible_move : this->m_possible_moves) {
			if (widget_coords == possible_move) {
				CursorManager::set_style(CursorManager::Hand);
				widget->set_background_color(widget_color);
				return;
			}
		}

		if (this->m_board.can_select(widget_coords)) {
			CursorManager::set_style(CursorManager::Hand);
			widget->set_background_color(widget_color);
			return;
		}
	};
	auto unhover_function = [this](Widget* widget, const vec2c& widget_coords) {
		CursorManager::set_style(CursorManager::Normal);
		widget->set_background_color(sf::Color::Transparent);
	};
	auto press_function = [this](Widget* widget, const vec2c& widget_coords) {
		for (const auto& possible_move : this->m_possible_moves) {
			if (widget_coords == possible_move) {
				this->m_board.move_figure(m_selected_cell, widget_coords);

				this->set_cell_highlight(this->m_selected_widget, HighlightDisabled);

				for (const auto& possible_move : this->m_possible_moves) {
					this->set_cell_highlight(this->m_cell_widgets[possible_move.x * 8 + possible_move.y].get(), HighlightDisabled);
				}

				this->m_selected_cell = vec2c(-1, -1);
				this->m_selected_widget = nullptr;
				this->m_possible_moves.clear();

				return;
			}
		}

		if (this->m_board.can_select(widget_coords)) {
			if (this->m_selected_widget != nullptr) {
				this->set_cell_highlight(this->m_selected_widget, HighlightDisabled);

				for (const auto& possible_move : this->m_possible_moves) {
					this->set_cell_highlight(this->m_cell_widgets[possible_move.x * 8 + possible_move.y].get(), HighlightDisabled);
				}
			}

			this->set_cell_highlight(widget, HighlightSelected);

			this->m_selected_cell = widget_coords;
			this->m_selected_widget = widget;
			this->m_possible_moves = this->m_board.get_possible_moves(widget_coords.x, widget_coords.y);

			for (const auto& possible_move : this->m_possible_moves) {
				this->set_cell_highlight(this->m_cell_widgets[possible_move.x * 8 + possible_move.y].get(), HighlightPossible);
			}
		}
		else if (this->m_selected_widget != nullptr) {
			this->set_cell_highlight(this->m_selected_widget, HighlightDisabled);

			for (const auto& possible_move : this->m_possible_moves) {
				this->set_cell_highlight(this->m_cell_widgets[possible_move.x * 8 + possible_move.y].get(), HighlightDisabled);
			}

			this->m_selected_cell = vec2c(-1, -1);
			this->m_selected_widget = nullptr;
			this->m_possible_moves.clear();
		}
	};

	auto release_function = [this](Widget* widget, const vec2c& widget_coords) {
		CursorManager::set_style(CursorManager::Normal);
	};

	for (unsigned int i = 0; i < 8; ++i) {
		for (unsigned int j = 0; j < 8; ++j) {
			std::shared_ptr<Widget> cell_widget = m_gui.create<Widget>();
			cell_widget->set_size(m_board.get_cell_size());

			vec2c widget_coords;
			if (m_player_type == WhitePlayer) {
				widget_coords = vec2c(i, 7 - j);
			}
			else {
				widget_coords = vec2c(7 - i, j);
			}

			cell_widget->bind(Widget::Hover, std::bind(hover_function, std::placeholders::_1, widget_coords));
			cell_widget->bind(Widget::Unhover, std::bind(unhover_function, std::placeholders::_1, widget_coords));
			cell_widget->bind(Widget::Press, std::bind(press_function, std::placeholders::_1, widget_coords));
			cell_widget->bind(Widget::Release, std::bind(release_function, std::placeholders::_1, widget_coords));

			board_layout->add_widget(cell_widget);
			m_cell_widgets[widget_coords.x * 8 + widget_coords.y] = cell_widget;
		}
	}

	m_gui.get_root_widget()->get_layout()->add_widget(m_widget_board);

	// Update layout
	vec2 window_size = vec2(Core::get_window()->getSize());
	resized(window_size.x, window_size.y);
}

void GameSession::set_cell_highlight(Widget * widget, HighlightType highlight_type)
{
	switch (highlight_type)
	{
	case GameSession::HighlightDisabled:
		widget->set_outline_color(sf::Color::Transparent);
		widget->set_size(m_board.get_cell_size());
		widget->set_position(widget->get_position() - vec2(1.5f, 1.5f) * widget->get_outline_thickness());
		widget->set_outline_thickness(0.0f);
		break;


	case GameSession::HighlightSelected:
		widget->set_outline_color(sf::Color(230, 255, 10));
		widget->set_outline_thickness(2.0f);
		widget->set_size(m_board.get_cell_size() - vec2(3.0f, 3.0f) * widget->get_outline_thickness());
		widget->set_position(widget->get_position() + vec2(1.5f, 1.5f) * widget->get_outline_thickness());
		break;


	case GameSession::HighlightPossible:
		widget->set_outline_color(sf::Color(0, 200, 10));
		widget->set_outline_thickness(2.0f);
		widget->set_size(m_board.get_cell_size() - vec2(3.0f, 3.0f) * widget->get_outline_thickness());
		widget->set_position(widget->get_position() + vec2(1.5f, 1.5f) * widget->get_outline_thickness());
		break;
	}
}
