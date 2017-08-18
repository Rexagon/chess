#include "MainMenu.h"

#include "Core.h"
#include "SHA256.h"

#include "RoomsList.h"

MainMenu::MainMenu()
{
	sf::Texture* background_texture = AssetManager::get<sf::Texture>("menu_background");
	background_texture->setSmooth(true);
	m_background_sprite.setTexture(*background_texture, true);
	m_background_sprite.setOrigin(vec2(background_texture->getSize()) / 2.0f);

	// Creating logo
	sf::Texture* logo_texture = AssetManager::get<sf::Texture>("menu_logo");
	logo_texture->setSmooth(true);
	m_logo.setTexture(*logo_texture, true);

	// Creating form label
	m_label_form = create_label(L"Авторизация");
	m_label_form->set_background_color(sf::Color(30, 30, 30, 220));
	m_label_form->set_alignment(GUI::AlignHCenter | GUI::AlignTop);
	m_label_form->set_enabled(false);
	m_label_form->set_padding(9.0f);
	m_label_form->set_font_size(18);

	// Login input label
	m_label_login = create_label(L"Логин");

	// Creating login input
	m_input_login = create_textbox(false);
	m_input_login->set_text(ConfigManager::get<std::wstring>("login"));
	m_gui.focus_item(m_input_login);

	// Creating password label
	m_label_password = create_label(L"Пароль");
	
	// Creating password input
	m_input_password = create_textbox(true);
	m_input_password->set_text(ConfigManager::get<std::wstring>("password"));

	// Creating password repeat label
	m_label_password_repeat = create_label(L"Повторите пароль");
	m_label_password_repeat->set_visible(false);

	// Creating password repeat input
	m_input_password_repeat = create_textbox(true);
	m_input_password_repeat->set_visible(false);

	// Creating login button
	m_button_login = create_button(L"Войти");
	m_button_login->bind(Widget::Press, [this](Widget* widget) {
		std::thread(&MainMenu::make_authorization, this).detach();
	});

	// Creating register button
	m_button_register = create_button(L"Регистрация");

	m_button_register->bind(Widget::Press, [this](Widget* widget) {
		switch(this->m_state) {
		case MenuState::Authorization:
			this->update_menu_state(MenuState::Registration);
			break;

		case MenuState::Registration:
			std::thread(&MainMenu::make_registration, this).detach();
			break;
		}
	});

	// Creating exit button
	m_button_exit = create_button(L"Выход");
	m_button_exit->bind(Widget::Press, [this](Widget* widget) {
		switch (this->m_state)
		{
		case MenuState::Authorization:
			Core::stop();
			break;

		case MenuState::Registration:
			this->update_menu_state(MenuState::Authorization);
			break;
		}
	});

	// Creating message label
	m_label_message = create_label(L"");
	m_label_message->set_color(sf::Color(250, 10, 10));
	m_label_message->set_font_size(14);
	m_label_message->set_wordwrap_enabled(true);
	m_label_message->set_text_outline_thickness(3.0f);
	m_label_message->set_text_outline_color(sf::Color::Black);

	// Update layout
	vec2 window_size = vec2(Core::get_window()->getSize());
	resized(window_size.x, window_size.y);
}

MainMenu::~MainMenu()
{
}

void MainMenu::update(const float dt)
{
	m_gui.update(dt);

	if (Input::get_key_down(Key::Escape)) {
		Core::delete_state();
		return;
	}

	if (Input::get_key_down(Key::Tab) || Input::get_key_down(Key::Return)) {
		Widget* focusedItem = m_gui.get_focused_item();

		if (focusedItem == m_input_login.get()) {
			m_gui.focus_item(m_input_password);
		}
		else if (focusedItem == m_input_password.get())
		{
			if (m_state == MenuState::Authorization) {
				m_gui.focus_item(m_button_login);
			}
			else if (m_state == MenuState::Registration) {
				m_gui.focus_item(m_input_password_repeat);
			}
		}
		else if (focusedItem == m_input_password_repeat.get()) {
			m_gui.focus_item(m_button_register);
		}
		else {
			m_gui.focus_item(m_input_login);
		}
	}
}

void MainMenu::resized(float width, float height)
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

	vec2 form_size(310.0f, line_height * 12.0f);
	vec2 form_position((vec2(width, height) - form_size) / 2.0f);

	form_position.x = ceilf(form_position.x);
	form_position.y = ceilf(form_position.y);

	vec2 logo_size(m_logo.getLocalBounds().width, m_logo.getLocalBounds().height);
	m_logo.setPosition((width - logo_size.x) / 2.0f, form_position.y - logo_size.y);

	m_label_form->set_position(form_position.x, form_position.y + line_height);
	m_label_form->set_size(form_size.x, form_size.y);

	m_label_login->set_position(form_position.x + 10.0f, form_position.y + line_height * 2.0f);
	m_label_login->set_size(form_size.x - 10.0f, line_height);

	m_input_login->set_position(form_position.x + 15.0f, form_position.y + line_height * 3.0f);
	m_input_login->set_size(form_size.x - 30.0f, line_height);

	m_label_password->set_position(form_position.x + 10.0f, form_position.y + line_height * 4.5f);
	m_label_password->set_size(form_size.x - 10.0f, line_height);

	m_input_password->set_position(form_position.x + 15.0f, form_position.y + line_height * 5.5f);
	m_input_password->set_size(form_size.x - 30.0f, line_height);

	m_label_password_repeat->set_position(form_position.x + 10.0f, form_position.y + line_height * 7.0f);
	m_label_password_repeat->set_size(form_size.x - 10.0f, line_height);

	m_input_password_repeat->set_position(form_position.x + 15.0f, form_position.y + line_height * 8.0f);
	m_input_password_repeat->set_size(form_size.x - 30.0f, line_height);

	m_button_login->set_position(form_position.x + 15.0f, form_position.y + line_height * 8.5f);
	m_button_login->set_size(form_size.x - 30.0f, line_height);

	m_button_register->set_position(form_position.x + 15.0f, form_position.y + line_height * 10.0f);
	m_button_register->set_size(form_size.x - 30.0f, line_height);

	m_button_exit->set_position(form_position.x + 15.0f, form_position.y + line_height * 11.5f);
	m_button_exit->set_size(form_size.x - 30.0f, line_height);

	m_label_message->set_position(form_position.x, form_position.y + line_height * 13.0f);
	m_label_message->set_size(form_size.x, line_height);
}

void MainMenu::draw(const float dt)
{
	Core::draw(m_background_sprite);
	Core::draw(m_logo);

	{
		std::unique_lock<std::recursive_mutex> lock(m_gui_mutex);
		m_gui.draw();
	}
}

void MainMenu::scene_leave()
{
}

void MainMenu::scene_return()
{
	// Update layout
	vec2 window_size = vec2(Core::get_window()->getSize());
	resized(window_size.x, window_size.y);
}

void MainMenu::update_menu_state(MenuState state)
{
	std::unique_lock<std::recursive_mutex> lock(m_gui_mutex);

	m_state = state;

	switch (m_state)
	{
	case MainMenu::MenuState::Authorization:
		m_label_form->set_text(L"Авторизация");
		m_button_login->set_visible(true);
		m_label_password_repeat->set_visible(false);
		m_input_password_repeat->set_visible(false);
		m_button_register->set_text(L"Регистрация");
		m_button_exit->set_text(L"Выход");
		m_label_message->set_text(L"");
		break;

	case MainMenu::MenuState::ProcessiogAuthorization:
		break;

	case MainMenu::MenuState::Registration:
		m_label_form->set_text(L"Регистрация");
		m_button_login->set_visible(false);
		m_label_password_repeat->set_visible(true);
		m_input_password_repeat->set_visible(true);
		m_input_password_repeat->set_text(L"");
		m_button_register->set_text(L"Подтвердить");
		m_button_exit->set_text(L"Назад");
		m_label_message->set_text(L"");
		break;

	case MainMenu::MenuState::PrecessingRegistration:
		break;
	}
}

void MainMenu::show_error(const std::wstring & text)
{
	std::unique_lock<std::recursive_mutex> lock(m_gui_mutex);
	m_label_message->set_color(sf::Color(250, 10, 10));
	m_label_message->set_text(text);
}

void MainMenu::show_info(const std::wstring & text)
{
	std::unique_lock<std::recursive_mutex> lock(m_gui_mutex);
	m_label_message->set_color(sf::Color::Green);
	m_label_message->set_text(text);
}

void MainMenu::make_authorization()
{
	sf::String login = m_input_login->get_text();
	sf::String password = m_input_password->get_text();

	if (login.getSize() < 4 || login.getSize() > 16) {
		this->show_error(L"Длина логина должна быть в пределах от 4 до 16 символов");
		return;
	}
	else if (password.getSize() < 4 || password.getSize() > 16) {
		this->show_error(L"Длина пароля должна быть в пределах от 4 до 16 символов");
		return;
	}

	m_state = MenuState::ProcessiogAuthorization;

	Client::add_command_handler(CommandPacket::Signin, [this, login, password](const CommandPacket& packet) {
		update_menu_state(MenuState::Authorization);

		if (packet.get_arguments()[0] == L"1") {
			ConfigManager::add("login", login.toWideString());
			ConfigManager::add("password", password.toWideString());
			Client::set_login(login.toWideString());
			Core::add_state<RoomsList>();
		}
		else {
			this->show_error(L"Неправильный логин или пароль");
		}
	});

	CommandPacket packet(CommandPacket::Signin, {login,	password});
	Client::send_command(packet, [this](sf::Socket::Status status) {
		Log::write("Signin packet send with status", status);
		if (status != sf::Socket::Done) {
			this->m_state = MenuState::Authorization;
			this->show_error(L"Произошла ошибка подключения");
		}
	});
}

void MainMenu::make_registration()
{
	sf::String login = m_input_login->get_text();
	sf::String password = m_input_password->get_text();
	sf::String password_repeat = m_input_password_repeat->get_text();

	if (login.getSize() < 4 || login.getSize() > 16) {
		this->show_error(L"Длина логина должна быть в пределах от 4 до 16 символов");
		return;
	}
	else if (password.getSize() < 4 || password.getSize() > 16) {
		this->show_error(L"Длина пароля должна быть в пределах от 4 до 16 символов");
		return;
	}

	else if (password != password_repeat) {
		this->show_error(L"Введённые пароли не совпадают");
		return;
	}

	Client::add_command_handler(CommandPacket::Signup, [this](const CommandPacket& packet) {
		if (packet.get_arguments()[0] == L"1") {
			this->update_menu_state(MenuState::Authorization);
			this->m_gui.focus_item(nullptr);
			this->show_info(L"Вы успешно зарегистрировались");
		}
		else {
			this->show_error(L"Такой пользователь уже есть");
		}
	});

	CommandPacket packet(CommandPacket::Signup, {
		m_input_login->get_text(),
		m_input_password->get_text()
	});
	Client::send_command(packet, [](sf::Socket::Status status) {
		Log::write("Signup packet send with status", status);
	});
}

std::shared_ptr<Label> MainMenu::create_label(const std::wstring & text)
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

std::shared_ptr<Label> MainMenu::create_button(const std::wstring & text)
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

std::shared_ptr<TextBox> MainMenu::create_textbox(bool masked)
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
