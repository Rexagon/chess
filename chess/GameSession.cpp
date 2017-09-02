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
	m_label_form = create_label(L"Информация");
	m_label_form->set_background_color(sf::Color(30, 30, 30, 220));
	m_label_form->set_alignment(GUI::AlignHCenter | GUI::AlignTop);
	m_label_form->set_enabled(false);
	m_label_form->set_padding(9.0f);
	m_label_form->set_font_size(18);

	m_label_white_player = create_label(L"За белых: ");
	m_label_black_player = create_label(L"За чёрных: ");
	m_label_turn = create_label(L"Сейчас ходят..");

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
			}
			else if (Client::get_login() == m_black_player) {
				m_player_type = BlackPlayer;
				m_board.set_main_color(Figure::Color::Black);
			}
			else {
				m_player_type = Spectator;
				m_board.set_main_color(Figure::Color::White);
			}

			m_label_white_player->set_text(L"За белых: " + m_white_player);
			m_label_black_player->set_text(L"За чёрных: " + m_black_player);

			m_is_private = arguments[5] == L"1";
			m_is_chat_enabled = arguments[6] == L"1";
			m_can_change_settings = arguments[2] == Client::get_login();

			Client::send_command(CommandPacket(CommandPacket::BoardState, { m_room_name }));
		}
	}, true);

	Client::add_command_handler(CommandPacket::BoardState, [this](const CommandPacket& packet) {
		this->m_board.init_field(packet);

		if (m_board.get_turn() == Figure::Color::White) {
			this->m_label_turn->set_text(L"Сейчас ходят белые");
		}
		else {
			this->m_label_turn->set_text(L"Сейчас ходят чёрные");
		}

		if (this->m_player_type != Spectator) {
			this->init_board_gui();
		}
	});

	Client::add_command_handler(CommandPacket::FigureMove, [this](const CommandPacket& packet) {
		std::unique_lock<std::recursive_mutex> lock(m_gui_mutex);

		std::vector<std::wstring> arguments = packet.get_arguments();
		if (arguments.size() > 1 && arguments[0] == m_room_name) {
			switch (arguments.size()) {
			case 2:
				if (arguments[1].size() == 2) {
					char last_pos = static_cast<char>(arguments[1][0]);
					char next_pos = static_cast<char>(arguments[1][1]);
				
					this->make_move(Figure::unpack_position(last_pos), Figure::unpack_position(next_pos));
				}
				break;

			case 3:
				if (arguments[1].size() == 2 && arguments[2].size() == 1) {
					vec2c pawn_last_pos = Figure::unpack_position(static_cast<char>(arguments[1][0]));
					vec2c pawn_next_pos = Figure::unpack_position(static_cast<char>(arguments[1][1]));

					this->make_move(pawn_last_pos, pawn_next_pos);

					this->m_board.get_figure(pawn_next_pos)->set_type(static_cast<char>(arguments[2][0]));
				}
				break;

			case 4:
				if (arguments[1].size() == 2 && arguments[2].size() == 2) {
					char king_last_pos = static_cast<char>(arguments[1][0]);
					char king_next_pos = static_cast<char>(arguments[1][1]);

					char rook_last_pos = static_cast<char>(arguments[2][0]);
					char rook_next_pos = static_cast<char>(arguments[2][1]);

					this->make_move(Figure::unpack_position(king_last_pos), Figure::unpack_position(king_next_pos));
					this->make_move(Figure::unpack_position(rook_last_pos), Figure::unpack_position(rook_next_pos));
				}
				break;

			default:
				break;
			}

			this->m_board.end_turn();
		}
	}, false);

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

	{
		std::unique_lock<std::recursive_mutex> lock(m_gui_mutex);

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

	float line_height = 32.0f;

	vec2 form_size(310.0f, line_height * 12.0f);
	vec2 form_position(width - form_size.x - 100.0f, board_position.y - board_size.y / 2.0f - 15.0f / 2.0f);

	form_position.x = ceilf(form_position.x);
	form_position.y = ceilf(form_position.y);

	m_label_form->set_position(form_position.x, form_position.y + line_height);
	m_label_form->set_size(form_size.x, form_size.y);

	m_label_white_player->set_position(form_position.x + 10.0f, form_position.y + line_height * 2.0f);
	m_label_white_player->set_size(form_size.x - 10.0f, line_height);

	m_label_black_player->set_position(form_position.x + 10.0f, form_position.y + line_height * 3.0f);
	m_label_black_player->set_size(form_size.x - 10.0f, line_height);

	m_label_turn->set_position(form_position.x + 10.0f, form_position.y + line_height * 4.5f);
	m_label_turn->set_size(form_size.x - 10.0f, line_height);
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
				Figure* figure = this->m_board.get_figure(this->m_selected_cell);

				std::vector<std::wstring> arguments = { m_room_name };

				if (figure->get_type() == Figure::Type::King &&
					widget_coords - this->m_selected_cell == vec2c(2, 0))
				{
					arguments.push_back({
						static_cast<wchar_t>(Figure::pack_position(this->m_selected_cell)),
						static_cast<wchar_t>(Figure::pack_position(widget_coords))
					});

					arguments.push_back({
						static_cast<wchar_t>(Figure::pack_position(vec2c(7, figure->get_position().y))),
						static_cast<wchar_t>(Figure::pack_position(vec2c(5, figure->get_position().y))),
					});

					arguments.push_back(std::wstring());
				}
				else if (figure->get_type() == Figure::Type::King &&
					widget_coords - this->m_selected_cell == vec2c(-2, 0))
				{
					arguments.push_back({
						static_cast<wchar_t>(Figure::pack_position(this->m_selected_cell)),
						static_cast<wchar_t>(Figure::pack_position(widget_coords))
					});

					arguments.push_back({
						static_cast<wchar_t>(Figure::pack_position(vec2c(0, figure->get_position().y))),
						static_cast<wchar_t>(Figure::pack_position(vec2c(3, figure->get_position().y))),
					});

					arguments.push_back(std::wstring());
				}
				else {
					arguments.push_back({
						static_cast<wchar_t>(Figure::pack_position(this->m_selected_cell)),
						static_cast<wchar_t>(Figure::pack_position(widget_coords))
					});
				}

				CommandPacket packet(CommandPacket::FigureMove, arguments);
				Client::send_command(packet);

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

void GameSession::make_move(const vec2c last_pos, const vec2c& next_pos)
{
	std::unique_lock<std::recursive_mutex> lock(m_gui_mutex);

	this->m_board.move_figure(last_pos, next_pos);

	if (m_board.get_turn() == Figure::Color::Black) {
		this->m_label_turn->set_text(L"Сейчас ходят белые");
	}
	else {
		this->m_label_turn->set_text(L"Сейчас ходят чёрные");
	}

	if (m_selected_widget != nullptr) {
		this->set_cell_highlight(this->m_selected_widget, HighlightDisabled);

		for (const auto& possible_move : this->m_possible_moves) {
			this->set_cell_highlight(this->m_cell_widgets[possible_move.x * 8 + possible_move.y].get(), HighlightDisabled);
		}

		this->m_selected_cell = vec2c(-1, -1);
		this->m_selected_widget = nullptr;
		this->m_possible_moves.clear();
	}
}

std::shared_ptr<Label> GameSession::create_label(const std::wstring & text)
{
	std::shared_ptr<Label> label = m_gui.create<Label>();
	label->set_background_color(sf::Color::Transparent);
	label->set_color(sf::Color::White);
	label->set_alignment(GUI::AlignLeft | GUI::AlignTop);
	label->set_padding(10.0f);
	label->set_text(text);
	label->set_font_size(16);

	m_gui.get_root_widget()->get_layout()->add_widget(label);

	return std::move(label);
}

std::shared_ptr<Label> GameSession::create_button(const std::wstring & text)
{
	std::shared_ptr<Label> button = m_gui.create<Label>();
	button->set_background_color(sf::Color(180, 180, 180, 255));
	button->set_color(sf::Color::Black);
	button->set_alignment(GUI::AlignHCenter | GUI::AlignTop);
	button->set_padding(10.0f);
	button->set_font_size(16);
	button->set_text(text);
	button->bind(Widget::Hover, [](Widget* widget) {
		CursorManager::set_style(CursorManager::Hand);
		widget->set_background_color(sf::Color(160, 160, 160, 255));
	});
	button->bind(Widget::Unhover, [](Widget* widget) {
		CursorManager::set_style(CursorManager::Normal);
		widget->set_background_color(sf::Color(180, 180, 180, 255));
	});
	button->bind(Widget::Focus, [](Widget* widget) {
		widget->set_outline_color(sf::Color(200, 200, 255, 200));
	});
	button->bind(Widget::Unfocus, [](Widget* widget) {
		widget->set_outline_color(sf::Color(120, 120, 120, 200));
	});

	m_gui.get_root_widget()->get_layout()->add_widget(button);

	return std::move(button);
}

std::shared_ptr<TextBox> GameSession::create_textbox(bool masked)
{
	std::shared_ptr<TextBox> text_box = m_gui.create<TextBox>();
	text_box->set_background_color(sf::Color(60, 60, 60, 60));
	text_box->set_color(sf::Color::White);
	text_box->set_outline_color(sf::Color(120, 120, 120, 200));
	text_box->set_outline_thickness(1.0f);
	text_box->set_alignment(GUI::AlignLeft | GUI::AlignTop);
	text_box->set_padding(10.0f);
	text_box->set_font_size(16);
	text_box->set_max_text_length(16);
	if (masked) {
		text_box->set_masking_character_enabled(true);
		text_box->set_masking_character('*');
	}
	text_box->bind(Widget::Hover, [](Widget* widget) {
		CursorManager::set_style(CursorManager::Text);

		if (!widget->is_focused()) {
			widget->set_outline_color(sf::Color(160, 160, 160, 200));
		}
	});
	text_box->bind(Widget::Unhover, [](Widget* widget) {
		CursorManager::set_style(CursorManager::Normal);

		if (!widget->is_focused()) {
			widget->set_outline_color(sf::Color(120, 120, 120, 200));
		}
	});
	text_box->bind(Widget::Focus, [](Widget* widget) {
		widget->set_outline_color(sf::Color(200, 200, 200, 200));
	});
	text_box->bind(Widget::Unfocus, [](Widget* widget) {
		widget->set_outline_color(sf::Color(120, 120, 120, 200));
	});

	m_gui.get_root_widget()->get_layout()->add_widget(text_box);

	return std::move(text_box);
}
