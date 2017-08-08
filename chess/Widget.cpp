#include "Widget.h"

#include "Core.h"
#include "GUI.h"

unsigned int Widget::CURRENT_ID = 0;

char Widget::get_type() const
{
	return GUI::WidgetType::EmptyWidget;
}

Widget::Widget() :
	m_parent(nullptr), m_layout(nullptr), m_is_enabled(true), m_is_visible(true), 
	m_id(CURRENT_ID++), m_minimum_size(0.0f, 0.0f), m_maximum_size(16384.0f, 16384.0f)
{
	Log::write("Widget", m_id, "created");
	m_actions.resize(ActionsNum, [](Widget*) {});
}

Widget::~Widget()
{
	delete m_layout;
	Log::write("Widget", m_id, "deleted");
}

void Widget::set_parent(Widget* widget)
{
	m_parent = widget;
}

Widget* Widget::get_parent() const
{
	return m_parent;
}

void Widget::on_draw()
{
	if (m_is_visible) {
		Core::get_window()->draw(m_geometry);
	}
}

void Widget::bind(int action, std::function<void(Widget*)> function)
{
	if (action > -1 && action < ActionsNum) {
		m_actions[action] = function;
	}
}

void Widget::trigger(int action)
{
	if (action > -1 && action < ActionsNum && m_is_visible) {
		m_actions[action](this);
	}
}

void Widget::unbind(int action)
{
	if (action > -1 && action < ActionsNum) {
		m_actions[action] = [](Widget*) {};
	}
}

void Widget::set_layout(Layout * layout)
{
	delete m_layout;
	m_layout = layout;
    if (m_layout != nullptr) {
        m_layout->m_parent = this;
    }
}

Layout * Widget::get_layout() const
{
	return m_layout;
}

void Widget::set_minimum_size(float width, float height)
{
	set_minimum_size(vec2(width, height));
}

void Widget::set_minimum_size(const vec2 & size)
{
	m_minimum_size = size;

	bool changed = false;
	vec2 currentSize = m_geometry.getSize();
	if (currentSize.x < m_minimum_size.x) {
		currentSize.x = m_minimum_size.x;
	}
	if (currentSize.y < m_minimum_size.y) {
		currentSize.y = m_minimum_size.y;
	}
	if (changed) {
		set_size(currentSize.x, currentSize.y);
	}
}

vec2 Widget::get_minimum_size() const
{
	return m_minimum_size;
}

void Widget::set_maximum_size(float width, float height)
{
	set_maximum_size(vec2(width, height));
}

void Widget::set_maximum_size(const vec2 & size)
{
	m_maximum_size = size;

	bool changed = false;
	vec2 currentSize = m_geometry.getSize();
	if (currentSize.x > m_maximum_size.x) {
		currentSize.x = m_maximum_size.x;
	}
	if (currentSize.y > m_maximum_size.y) {
		currentSize.y = m_maximum_size.y;
	}
	if (changed) {
		set_size(currentSize.x, currentSize.y);
	}
}

vec2 Widget::get_maximum_size() const
{
	return m_maximum_size;
}

void Widget::set_fixed_width(float width)
{
	m_minimum_size.x = width;
	m_maximum_size.x = width;
	set_size(width, m_geometry.getSize().y);
}

void Widget::set_fixed_height(float height)
{
	m_minimum_size.y = height;
	m_maximum_size.y = height;
	set_size(m_geometry.getSize().x, height);
}

void Widget::set_fixed_size(float width, float height)
{
	set_minimum_size(width, height);
	set_maximum_size(width, height);
	set_size(width, height);
}

void Widget::set_fixed_size(const vec2 & size)
{
	set_fixed_size(size.x, size.y);
}

void Widget::set_size(float width, float height)
{
	if (width < m_minimum_size.x) {
		width = m_minimum_size.x;
	}

	if (width > m_maximum_size.x) {
		width = m_maximum_size.x;
	}

	if (height < m_minimum_size.y) {
		height = m_minimum_size.y;
	}

	if (height > m_maximum_size.y) {
		height = m_maximum_size.y;
	}

	m_geometry.setSize(sf::Vector2f(width, height));
	update();
}

void Widget::set_size(const vec2 & size)
{
	set_size(size.x, size.y);
}

vec2 Widget::get_size() const
{
	sf::Vector2f size = m_geometry.getSize();
	return vec2(size.x, size.y);
}

void Widget::set_position(float x, float y)
{
	m_geometry.setPosition(x, y);
	update();
}

void Widget::set_position(const vec2 & position)
{
	set_position(position.x, position.y);
}

vec2 Widget::get_position() const
{
	sf::Vector2f position = m_geometry.getPosition();
	return vec2(position.x, position.y);
}

sf::RectangleShape Widget::get_geometry() const
{
	return m_geometry;
}

sf::FloatRect Widget::get_rect() const
{
	return m_geometry.getGlobalBounds();
}

void Widget::set_origin(const vec2 & origin)
{
	m_geometry.setOrigin(origin.x, origin.y);
	update();
}

vec2 Widget::get_origin() const
{
	return vec2(m_geometry.getOrigin().x, m_geometry.getOrigin().y);
}

void Widget::set_background_color(const sf::Color & color)
{
	m_geometry.setFillColor(color);
}

sf::Color Widget::get_background_color() const
{
	return m_geometry.getFillColor();
}

void Widget::set_outline_color(const sf::Color & color)
{
	m_geometry.setOutlineColor(color);
}

void Widget::set_outline_thickness(float thickness)
{
	m_geometry.setOutlineThickness(thickness);
}

void Widget::set_texture(const sf::Texture * texture)
{
	m_geometry.setTexture(texture);
	m_geometry.setTextureRect(recti(m_geometry.getPosition().x, m_geometry.getPosition().y,
		m_geometry.getSize().x, m_geometry.getSize().y));
}

void Widget::set_enabled(bool enabled)
{
	bool was_enabled = m_is_enabled;
	m_is_enabled = enabled;
	if (was_enabled != m_is_enabled) {
		if (m_is_enabled) {
			m_actions[Enable](this);
		}
		else {
			m_actions[Disable](this);
		}
	}
}

void Widget::set_visible(bool visible)
{
	bool was_visible = m_is_visible;
	m_is_visible = visible;
	if (was_visible != m_is_visible) {
		if (m_is_visible) {
			m_actions[Show](this);
		}
		else {
			m_actions[Hide](this);
		}
	}
}
