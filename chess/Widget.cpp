#include "Widget.h"

#include "Core.h"
#include "GUI.h"

unsigned int Widget::CURRENT_ID = 0;

Widget::Widget() :
	m_parent(nullptr), m_layout(nullptr), m_isEnabled(true), m_isVisible(true), 
	m_id(CURRENT_ID++), m_minimumSize(0.0f, 0.0f), m_maximumSize(16384.0f, 16384.0f)
{
	Log::write("Widget", m_id, "created");
	m_actions.resize(ActionsNum, [](Widget*) {});
}

Widget::~Widget()
{
	delete m_layout;
	Log::write("Widget", m_id, "deleted");
}

void Widget::setParent(Widget* widget)
{
	m_parent = widget;
}

Widget* Widget::getParent() const
{
	return m_parent;
}

void Widget::onDraw()
{
	if (m_isVisible) {
		Core::getWindow()->draw(m_geometry);
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
	if (action > -1 && action < ActionsNum && m_isVisible) {
		m_actions[action](this);
	}
}

void Widget::unbind(int action)
{
	if (action > -1 && action < ActionsNum) {
		m_actions[action] = [](Widget*) {};
	}
}

void Widget::setLayout(Layout * layout)
{
	delete m_layout;
	m_layout = layout;
    if (m_layout != nullptr) {
        m_layout->m_parent = this;
    }
}

Layout * Widget::getLayout() const
{
	return m_layout;
}

void Widget::setMinimumSize(float width, float height)
{
	setMinimumSize(vec2(width, height));
}

void Widget::setMinimumSize(const vec2 & size)
{
	m_minimumSize = size;

	bool changed = false;
	vec2 currentSize = m_geometry.getSize();
	if (currentSize.x < m_minimumSize.x) {
		currentSize.x = m_minimumSize.x;
	}
	if (currentSize.y < m_minimumSize.y) {
		currentSize.y = m_minimumSize.y;
	}
	if (changed) {
		setSize(currentSize.x, currentSize.y);
	}
}

vec2 Widget::getMinimumSize() const
{
	return m_minimumSize;
}

void Widget::setMaximumSize(float width, float height)
{
	setMaximumSize(vec2(width, height));
}

void Widget::setMaximumSize(const vec2 & size)
{
	m_maximumSize = size;

	bool changed = false;
	vec2 currentSize = m_geometry.getSize();
	if (currentSize.x > m_maximumSize.x) {
		currentSize.x = m_maximumSize.x;
	}
	if (currentSize.y > m_maximumSize.y) {
		currentSize.y = m_maximumSize.y;
	}
	if (changed) {
		setSize(currentSize.x, currentSize.y);
	}
}

vec2 Widget::getMaximumSize() const
{
	return m_maximumSize;
}

void Widget::setFixedWidth(float width)
{
	m_minimumSize.x = width;
	m_maximumSize.x = width;
	setSize(width, m_geometry.getSize().y);
}

void Widget::setFixedHeight(float height)
{
	m_minimumSize.y = height;
	m_maximumSize.y = height;
	setSize(m_geometry.getSize().x, height);
}

void Widget::setFixedSize(float width, float height)
{
	setMinimumSize(width, height);
	setMaximumSize(width, height);
	setSize(width, height);
}

void Widget::setFixedSize(const vec2 & size)
{
	setFixedSize(size.x, size.y);
}

void Widget::setSize(float width, float height)
{
	if (width < m_minimumSize.x) {
		width = m_minimumSize.x;
	}

	if (width > m_maximumSize.x) {
		width = m_maximumSize.x;
	}

	if (height < m_minimumSize.y) {
		height = m_minimumSize.y;
	}

	if (height > m_maximumSize.y) {
		height = m_maximumSize.y;
	}

	m_geometry.setSize(sf::Vector2f(width, height));
	update();
}

void Widget::setSize(const vec2 & size)
{
	setSize(size.x, size.y);
}

vec2 Widget::getSize() const
{
	sf::Vector2f size = m_geometry.getSize();
	return vec2(size.x, size.y);
}

void Widget::setPosition(float x, float y)
{
	m_geometry.setPosition(x, y);
	update();
}

void Widget::setPosition(const vec2 & position)
{
	setPosition(position.x, position.y);
}

vec2 Widget::getPosition() const
{
	sf::Vector2f position = m_geometry.getPosition();
	return vec2(position.x, position.y);
}

sf::RectangleShape Widget::getGeometry() const
{
	return m_geometry;
}

sf::FloatRect Widget::getRect() const
{
	return m_geometry.getGlobalBounds();
}

void Widget::setOrigin(const vec2 & origin)
{
	m_geometry.setOrigin(origin.x, origin.y);
	update();
}

vec2 Widget::getOrigin() const
{
	return vec2(m_geometry.getOrigin().x, m_geometry.getOrigin().y);
}

void Widget::setBackgroundColor(const sf::Color & color)
{
	m_geometry.setFillColor(color);
}

sf::Color Widget::getBackgroundColor() const
{
	return m_geometry.getFillColor();
}

void Widget::setTexture(const sf::Texture * texture)
{
	m_geometry.setTexture(texture);
	m_geometry.setTextureRect(recti(m_geometry.getPosition().x, m_geometry.getPosition().y,
		m_geometry.getSize().x, m_geometry.getSize().y));
}

void Widget::setEnabled(bool enabled)
{
	bool wasEnabled = m_isEnabled;
	m_isEnabled = enabled;
	if (wasEnabled != m_isEnabled) {
		if (m_isEnabled) {
			m_actions[Enable](this);
		}
		else {
			m_actions[Disable](this);
		}
	}
}

void Widget::setVisible(bool visible)
{
	bool wasVisible = m_isVisible;
	m_isVisible = visible;
	if (wasVisible != m_isVisible) {
		if (m_isVisible) {
			m_actions[Show](this);
		}
		else {
			m_actions[Hide](this);
		}
	}
}
