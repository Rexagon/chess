#include "GameSession.h"

#include "Core.h"

GameSession::GameSession(const std::wstring& name) :
	m_room_name(name), m_cell_widgets(64)
{
	sf::Texture* background_texture = AssetManager::get<sf::Texture>("game_background");
	background_texture->setSmooth(true);
	m_background_sprite.setTexture(*background_texture, true);
	m_background_sprite.setOrigin(vec2(background_texture->getSize()) / 2.0f);

	sf::Texture* board_texture = AssetManager::get<sf::Texture>("game_chess_board");
	board_texture->setSmooth(true);
	m_board_sprite.setTexture(*board_texture, true);
	m_board_sprite.setOrigin(vec2(board_texture->getSize()) / 2.0f);

	sf::Texture* figures_texture = AssetManager::get<sf::Texture>("game_figures");
	figures_texture->setSmooth(true);
	for (unsigned int i = 0; i < 6; ++i) {
		for (unsigned int j = 0; j < 2; ++j) {
			recti texture_rect(i * 128.0f, j * 128.0f, 128.0f, 128.0f);
			m_figure_sprites.push_back(sf::Sprite(*figures_texture, texture_rect));
		}
	}
	
	// Create GUI
	m_widget_board = m_gui.create<Widget>();
	Layout* board_layout = new Layout(m_widget_board);

	auto hover_function = [this](Widget* widget) {
		widget->set_background_color(sf::Color(100, 100, 100, 100));
	};
	auto unhover_function = [this](Widget* widget) {
		widget->set_background_color(sf::Color::Transparent);
	};
	auto press_function = [this](Widget* widget) {
		
	};

	for (unsigned int i = 0; i < m_cell_widgets.size(); ++i) {
		std::shared_ptr<Widget> cell_widget = m_gui.create<Widget>();

		cell_widget->set_outline_color(sf::Color::Transparent);
		cell_widget->set_outline_thickness(1.0f);

		cell_widget->bind(Widget::Hover, hover_function);
		cell_widget->bind(Widget::Unhover, unhover_function);
		cell_widget->bind(Widget::Press, press_function);

		board_layout->add_widget(cell_widget);
		m_cell_widgets[i] = cell_widget;
	}

	m_gui.get_root_widget()->get_layout()->add_widget(m_widget_board);


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
			std::wstring owner = arguments[2];
			std::wstring white_player = arguments[3];
			std::wstring black_player = arguments[4];
			bool is_private = arguments[5] == L"1";
			bool is_chat_enabled = arguments[6] == L"1";

			std::wcout <<
				"name:         " << m_room_name << "\n" <<
				"owner:        " << owner << "\n" <<
				"white_player: " << white_player << "\n" <<
				"black_player: " << black_player << "\n" <<
				"is_private:   " << is_private << "\n" <<
				"chat_enabled: " << is_chat_enabled << "\n";
		}
	});

	Client::send_command(CommandPacket(CommandPacket::RoomSettings, { m_room_name }));
}

void GameSession::update(const float dt)
{
	if (Input::get_key_down(Key::Escape)) {
		Core::delete_state();
		return;
	}
}

void GameSession::draw(const float dt)
{
	Core::draw(m_background_sprite);
	Core::draw(m_board_sprite);

	m_gui.draw();

	for (unsigned int i = 0; i < m_figure_sprites.size(); ++i) {
		Core::draw(m_figure_sprites[i]);
	}
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
	vec2 board_size = vec2(m_board_sprite.getTexture()->getSize());
	vec2 board_position = vec2((width - board_size.x) / 2.0f, height / 2.0f);
	if (board_position.x < (board_size.x + 20.0f) / 2.0f) {
		board_position.x = (board_size.x + 20.0f) / 2.0f;
	}
	board_position.x = ceilf(board_position.x);
	board_position.y = ceilf(board_position.y);

	m_board_sprite.setPosition(board_position);

	vec2 field_offset = vec2(54.0f, 54.0f) - board_size / 2.0f;
	field_offset.x = ceilf(field_offset.x);
	field_offset.y = ceilf(field_offset.y);

	float cell_width = 74.0f;

	for (unsigned int i = 0; i < 6; ++i) {
		for (unsigned int j = 0; j < 2; ++j) {
			m_figure_sprites[i * 2 + j].setScale(0.57f, 0.57f);
			m_figure_sprites[i * 2 + j].setPosition(board_position + field_offset + vec2(i, j) * cell_width);
		}
	}

	for (unsigned int i = 0; i < 8; ++i) {
		for (unsigned int j = 0; j < 8; ++j) {
			m_cell_widgets[i * 8 + j]->set_position(board_position + field_offset + vec2(i, j) * cell_width);
			m_cell_widgets[i * 8 + j]->set_size(cell_width, cell_width);
		}
	}
}
