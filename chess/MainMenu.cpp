#include "MainMenu.h"

#include "Core.h"

MainMenu::MainMenu()
{
	sf::Texture* background_texture = AssetManager::get<sf::Texture>("menu_background");
	background_texture->setSmooth(true);
	m_background_sprite.setTexture(*background_texture, true);
	m_background_sprite.setOrigin(vec2(background_texture->getSize()) / 2.0f);

	// Creating form label
	m_label_form = m_gui.create<Label>();
	m_label_form->set_enabled(false);
	m_label_form->set_background_color(sf::Color(30, 30, 30, 220));
	m_label_form->set_color(sf::Color::White);
	m_label_form->set_alignment(GUI::AlignHCenter | GUI::AlignTop);
	m_label_form->set_padding(9.0f);
	m_label_form->set_text(L"Авторизация");
	m_label_form->set_font_size(18);

	m_gui.get_root_widget()->get_layout()->add_widget(m_label_form);

	// Login input label
	m_label_login = m_gui.create<Label>();
	m_label_login->set_background_color(sf::Color::Transparent);
	m_label_login->set_color(sf::Color::White);
	m_label_login->set_alignment(GUI::AlignLeft | GUI::AlignTop);
	m_label_login->set_padding(10.0f);
	m_label_login->set_text(L"Логин");
	m_label_login->set_font_size(16);

	m_gui.get_root_widget()->get_layout()->add_widget(m_label_login);

	// Creating login input
	m_input_login = m_gui.create<TextBox>();
	m_input_login->set_background_color(sf::Color(60, 60, 60, 60));
	m_input_login->set_color(sf::Color::White);
	m_input_login->set_outline_color(sf::Color(120, 120, 120, 200));
	m_input_login->set_outline_thickness(1.0f);
	m_input_login->set_alignment(GUI::AlignLeft | GUI::AlignTop);
	m_input_login->set_padding(10.0f);
	m_input_login->set_font_size(16);
	m_input_login->set_max_text_length(16);
	m_input_login->bind(Widget::Hover, [](Widget* widget) {
		SystemCursor cursor(SystemCursor::Text);
		cursor.set(Core::get_window()->getSystemHandle());

		if (!widget->is_focused()) {
			widget->set_outline_color(sf::Color(160, 160, 160, 200));
		}
	});
	m_input_login->bind(Widget::Unhover, [](Widget* widget) {
		SystemCursor cursor(SystemCursor::Normal);
		cursor.set(Core::get_window()->getSystemHandle());

		if (!widget->is_focused()) {
			widget->set_outline_color(sf::Color(120, 120, 120, 200));
		}
	});
	m_input_login->bind(Widget::Focus, [](Widget* widget) {
		widget->set_outline_color(sf::Color(200, 200, 200, 200));
	});
	m_input_login->bind(Widget::Unfocus, [](Widget* widget) {
		widget->set_outline_color(sf::Color(120, 120, 120, 200));
	});

	m_gui.get_root_widget()->get_layout()->add_widget(m_input_login);

	// Creating password label
	m_label_password = m_gui.create<Label>();
	m_label_password->set_background_color(sf::Color::Transparent);
	m_label_password->set_color(sf::Color::White);
	m_label_password->set_alignment(GUI::AlignLeft | GUI::AlignTop);
	m_label_password->set_padding(10.0f);
	m_label_password->set_text(L"Пароль");
	m_label_password->set_font_size(16);

	m_gui.get_root_widget()->get_layout()->add_widget(m_label_password);
	
	// Creating password input
	m_input_password = m_gui.create<TextBox>();
	m_input_password->set_background_color(sf::Color(60, 60, 60, 60));
	m_input_password->set_color(sf::Color::White);
	m_input_password->set_outline_color(sf::Color(120, 120, 120, 200));
	m_input_password->set_outline_thickness(1.0f);
	m_input_password->set_alignment(GUI::AlignLeft | GUI::AlignTop);
	m_input_password->set_padding(10.0f);
	m_input_password->set_font_size(16);
	m_input_password->set_max_text_length(16);
	m_input_password->set_masking_character_enabled(true);
	m_input_password->set_masking_character('*');
	m_input_password->bind(Widget::Hover, [](Widget* widget) {
		SystemCursor cursor(SystemCursor::Text);
		cursor.set(Core::get_window()->getSystemHandle());

		if (!widget->is_focused()) {
			widget->set_outline_color(sf::Color(160, 160, 160, 200));
		}
	});
	m_input_password->bind(Widget::Unhover, [](Widget* widget) {
		SystemCursor cursor(SystemCursor::Normal);
		cursor.set(Core::get_window()->getSystemHandle());

		if (!widget->is_focused()) {
			widget->set_outline_color(sf::Color(120, 120, 120, 200));
		}
	});
	m_input_password->bind(Widget::Focus, [](Widget* widget) {
		widget->set_outline_color(sf::Color(200, 200, 200, 200));
	});
	m_input_password->bind(Widget::Unfocus, [](Widget* widget) {
		widget->set_outline_color(sf::Color(120, 120, 120, 200));
	});

	m_gui.get_root_widget()->get_layout()->add_widget(m_input_password);

	// Creating login button
	m_button_login = m_gui.create<Label>();
	m_button_login->set_background_color(sf::Color(180, 180, 180, 255));
	m_button_login->set_color(sf::Color::Black);
	m_button_login->set_alignment(GUI::AlignHCenter | GUI::AlignTop);
	m_button_login->set_padding(10.0f);
	m_button_login->set_font_size(16);
	m_button_login->set_text(L"Войти");
	m_button_login->bind(Widget::Hover, [](Widget* widget) {
		SystemCursor cursor(SystemCursor::Hand);
		cursor.set(Core::get_window()->getSystemHandle());
		widget->set_background_color(sf::Color(160, 160, 160, 255));
	});
	m_button_login->bind(Widget::Unhover, [](Widget* widget) {
		SystemCursor cursor(SystemCursor::Normal);
		cursor.set(Core::get_window()->getSystemHandle());
		widget->set_background_color(sf::Color(180, 180, 180, 255));
	});

	m_gui.get_root_widget()->get_layout()->add_widget(m_button_login);

	// Creating register button
	m_button_register = m_gui.create<Label>();
	m_button_register->set_background_color(sf::Color(180, 180, 180, 255));
	m_button_register->set_color(sf::Color::Black);
	m_button_register->set_alignment(GUI::AlignHCenter | GUI::AlignTop);
	m_button_register->set_padding(10.0f);
	m_button_register->set_font_size(16);
	m_button_register->set_text(L"Создать аккаунт");
	m_button_register->bind(Widget::Hover, [](Widget* widget) {
		SystemCursor cursor(SystemCursor::Hand);
		cursor.set(Core::get_window()->getSystemHandle());
		widget->set_background_color(sf::Color(160, 160, 160, 255));
	});
	m_button_register->bind(Widget::Unhover, [](Widget* widget) {
		SystemCursor cursor(SystemCursor::Normal);
		cursor.set(Core::get_window()->getSystemHandle());
		widget->set_background_color(sf::Color(180, 180, 180, 255));
	});

	m_gui.get_root_widget()->get_layout()->add_widget(m_button_register);



	vec2 window_size = vec2(Core::get_window()->getSize());
	resized(window_size.x, window_size.y);
}

MainMenu::~MainMenu()
{
}

void MainMenu::init()
{
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

	vec2 form_size(310.0f, line_height * 10.0f);
	vec2 form_position((vec2(width, height) - form_size) / 2.0f);

	form_position.x = ceilf(form_position.x);
	form_position.y = ceilf(form_position.y);

	m_label_form->set_position(form_position.x, form_position.y);
	m_label_form->set_size(form_size.x, form_size.y);

	m_label_login->set_position(form_position.x + 10.0f, form_position.y + line_height);
	m_label_login->set_size(form_size.x - 10.0f, line_height);

	m_input_login->set_position(form_position.x + 15.0f, form_position.y + line_height * 2.0f);
	m_input_login->set_size(form_size.x - 30.0f, line_height);

	m_label_password->set_position(form_position.x + 10.0f, form_position.y + line_height * 3.5f);
	m_label_password->set_size(form_size.x - 10.0f, line_height);

	m_input_password->set_position(form_position.x + 15.0f, form_position.y + line_height * 4.5f);
	m_input_password->set_size(form_size.x - 30.0f, line_height);

	m_button_login->set_position(form_position.x + 15.0f, form_position.y + line_height * 7.0f);
	m_button_login->set_size(form_size.x - 30.0f, line_height);

	m_button_register->set_position(form_position.x + 15.0f, form_position.y + line_height * 8.5f);
	m_button_register->set_size(form_size.x - 30.0f, line_height);
}

void MainMenu::update(const float dt)
{
	if (Input::get_key_down(Key::Escape)) {
		Core::delete_state();
		return;
	}
}

void MainMenu::draw(const float dt)
{
	Core::draw(m_background_sprite);
	m_gui.draw();
}
