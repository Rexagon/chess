#include "RoomsList.h"

#include "Core.h"
#include "GameSession.h"

RoomsList::RoomsList()
{
	sf::Texture* background_texture = AssetManager::get<sf::Texture>("rooms_background");
	background_texture->setSmooth(true);
	m_background_sprite.setTexture(*background_texture, true);
	m_background_sprite.setOrigin(vec2(background_texture->getSize()) / 2.0f);

	// Creating form list
	m_form_list = create_label(L"—писок сессий");
	m_form_list->set_background_color(sf::Color(30, 30, 30, 220));
	m_form_list->set_alignment(GUI::AlignHCenter | GUI::AlignTop);
	m_form_list->set_enabled(false);
	m_form_list->set_padding(9.0f);
	m_form_list->set_font_size(18);

	m_widget_list = m_gui.create<Widget>();
	m_widget_list->set_background_color(sf::Color(30, 30, 30, 220));
	Layout* layout = new Layout(m_widget_list);
	m_gui.get_root_widget()->get_layout()->add_widget(m_widget_list);

	// Update layout
	vec2 window_size = vec2(Core::get_window()->getSize());
	resized(window_size.x, window_size.y);
}

RoomsList::~RoomsList()
{
	Client::remove_command_handler(CommandPacket::ChangeRoom);
	Client::remove_command_handler(CommandPacket::RoomList);

	Client::send_command(CommandPacket(CommandPacket::Signout, {}));
}

void RoomsList::init()
{
	Client::add_command_handler(CommandPacket::ChangeRoom, [this](const CommandPacket& packet) {
		if (packet.get_arguments_count() > 0) {
			Core::add_state<GameSession>(packet.get_arguments()[0]);
		}
	}, false);

	Client::add_command_handler(CommandPacket::RoomList, [this](const CommandPacket& packet) {
		std::vector<std::wstring> arguments = packet.get_arguments();
		unsigned int arguments_count = arguments.size();

		if (packet.is_valid() && arguments_count % 4 == 0) {
			std::unique_lock<std::mutex> lock(m_rooms_mutex);
			m_rooms.clear();

			for (unsigned int i = 0; i < arguments_count; i += 4) {
				try {
					Room room;
					room.name = arguments[i + 0];
					room.white_player = arguments[i + 1];
					room.black_player = arguments[i + 2];
					room.spectators_count = std::stoi(arguments[i + 3]);

					m_rooms.push_back(room);
				}
				catch (const std::exception& e) {
					Log::write("unable to parse room #" + std::to_string(i));
				}
			}
		}

		this->update_rooms_list();
		vec2 window_size = vec2(Core::get_window()->getSize());
		resized(window_size.x, window_size.y);
	}, false);

	Client::send_command(CommandPacket(CommandPacket::RoomList, {}));
}

void RoomsList::update(const float dt)
{
	if (Input::get_key_down(Key::Escape)) {
		Core::delete_state();
	}
}

void RoomsList::draw(const float dt)
{
	Core::draw(m_background_sprite);
	m_gui.draw();
}

void RoomsList::resized(float width, float height)
{
	Core::get_window()->setView(sf::View(sf::FloatRect(0, 0, width, height)));

	vec2 background_scale(1.0f, 1.0f);
	vec2 background_size = vec2(m_background_sprite.getTexture()->getSize());

	m_background_sprite.setPosition(width / 2.0f, height / 2.0f);

	background_scale.x = width / background_size.x;
	background_scale.y = height / background_size.y;

	float scale = std::max(background_scale.x, background_scale.y);
	m_background_sprite.setScale(scale, scale);

	// GUI positioning
	float line_height = 32.0f;

	vec2 form_size(width * 0.9f, height * 0.8f);
	if (form_size.x > 1024.0f) {
		form_size.x = 1024.0f;
	}
	if (form_size.y > 768.0f) {
		form_size.y = 768.0f;
	}

	vec2 form_position((vec2(width, height) - form_size) / 2.0f);

	form_position.x = ceilf(form_position.x);
	form_position.y = ceilf(form_position.y);

	m_form_list->set_position(form_position.x, form_position.y);
	m_form_list->set_size(form_size.x, form_size.y);

	unsigned int rooms_per_page = m_rooms.size();

	m_widget_list->set_position(form_position.x, form_position.y + line_height + 10.0f);
	m_widget_list->set_size(form_size.x, (line_height + 10.0f) * rooms_per_page - 10.0f);

	for (unsigned int i = 0; i < rooms_per_page; ++i) {
		m_rooms_widgets[i]->set_position(form_position.x - 1.0f, form_position.y + (line_height + 10.0f) * (i + 1));
		m_rooms_widgets[i]->set_size(form_size.x + 2.0f, line_height);
	}
}

void RoomsList::update_rooms_list()
{
	std::unique_lock<std::mutex> lock(m_rooms_mutex);

	m_widget_list->get_layout()->clear();
	m_rooms_widgets.clear();

	for (Room room : m_rooms) {
		std::shared_ptr<Widget> widget = m_gui.create<Widget>();
		widget->set_background_color(sf::Color(20, 20, 20, 200));
		widget->set_outline_color(sf::Color(20, 20, 20, 200));
		widget->set_outline_thickness(1.0f);
		HorizontalLayout* widget_layout = new HorizontalLayout(widget);

		std::wstring players_string;
		if (room.white_player.size() == 0) {
			room.white_player = L"?";
		}
		if (room.black_player.size() == 0) {
			room.black_player = L"?";
		}
		players_string = room.white_player + L" Ч " + room.black_player;

		std::wstring spectators_string = L"«рителей: " + std::to_wstring(room.spectators_count);

		std::shared_ptr<Label> label_name = create_label(room.name);
		
		std::shared_ptr<Label> label_players = create_label(players_string);
		label_players->set_alignment(GUI::AlignHCenter);

		std::shared_ptr<Label> label_spectators = create_label(spectators_string);
		label_spectators->set_alignment(GUI::AlignRight);

		widget_layout->add_widget(label_name);
		widget_layout->add_widget(label_players);
		widget_layout->add_widget(label_spectators);

		auto hover_function = [widget](Widget*) {
			CursorManager::set_style(CursorManager::Hand);
			widget->set_outline_color(sf::Color(180, 180, 180, 200));
		};
		auto unhover_function = [widget](Widget*) {
			CursorManager::set_style(CursorManager::Normal);
			widget->set_outline_color(sf::Color(20, 20, 20, 200));
		};

		auto press_function = [widget, room](Widget*) {
			Client::send_command(CommandPacket(CommandPacket::ChangeRoom, {
				room.name
			}));
		};

		label_name->bind(Widget::Hover, hover_function);
		label_players->bind(Widget::Hover, hover_function);
		label_spectators->bind(Widget::Hover, hover_function);

		label_name->bind(Widget::Unhover, unhover_function);
		label_players->bind(Widget::Unhover, unhover_function);
		label_spectators->bind(Widget::Unhover, unhover_function);

		label_name->bind(Widget::Press, press_function);
		label_players->bind(Widget::Press, press_function);
		label_spectators->bind(Widget::Press, press_function);

		m_widget_list->get_layout()->add_widget(widget);
		m_rooms_widgets.push_back(widget);
	}
}

std::shared_ptr<Label> RoomsList::create_label(const std::wstring & text)
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

std::shared_ptr<Label> RoomsList::create_button(const std::wstring & text)
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

	m_gui.get_root_widget()->get_layout()->add_widget(button);

	return std::move(button);
}

std::shared_ptr<TextBox> RoomsList::create_textbox()
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
