#include "RoomsList.h"

#include "Core.h"

RoomsList::RoomsList()
{
	sf::Texture* background_texture = AssetManager::get<sf::Texture>("rooms_background");
	background_texture->setSmooth(true);
	m_background_sprite.setTexture(*background_texture, true);
	m_background_sprite.setOrigin(vec2(background_texture->getSize()) / 2.0f);

	// Creating form list
	m_form_list = create_label(L"Список сессий");
	m_form_list->set_background_color(sf::Color(30, 30, 30, 220));
	m_form_list->set_alignment(GUI::AlignHCenter | GUI::AlignTop);
	m_form_list->set_enabled(false);
	m_form_list->set_padding(9.0f);
	m_form_list->set_font_size(18);

	// Update layout
	vec2 window_size = vec2(Core::get_window()->getSize());
	resized(window_size.x, window_size.y);
}

RoomsList::~RoomsList()
{
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

	vec2 form_size(600.0f, line_height * 12.0f);
	vec2 form_position((vec2(width, height) - form_size) / 2.0f);

	form_position.x = ceilf(form_position.x);
	form_position.y = ceilf(form_position.y);

	m_form_list->set_position(form_position.x, form_position.y);
	m_form_list->set_size(form_size.x, form_size.y);
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
		SystemCursor cursor(SystemCursor::Hand);
		cursor.set(Core::get_window()->getSystemHandle());
		widget->set_background_color(sf::Color(160, 160, 160, 255));
	});
	button->bind(Widget::Unhover, [](Widget* widget) {
		SystemCursor cursor(SystemCursor::Normal);
		cursor.set(Core::get_window()->getSystemHandle());
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
		SystemCursor cursor(SystemCursor::Text);
		cursor.set(Core::get_window()->getSystemHandle());

		if (!widget->is_focused()) {
			widget->set_outline_color(sf::Color(160, 160, 160, 200));
		}
	});
	text_box->bind(Widget::Unhover, [](Widget* widget) {
		SystemCursor cursor(SystemCursor::Normal);
		cursor.set(Core::get_window()->getSystemHandle());

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
