#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Math.h"

class GUI;
class Layout;

class Widget
{
public:
	enum Action
	{
		Hover,
		Unhover,
		Press,
		Release,
		Focus,
		Unfocus,
		Enable,
		Disable,
		Show,
		Hide,

		ActionsNum
	};
	
	virtual ~Widget();

	void setParent(Widget* widget);
	Widget* getParent() const;

	void bind(int action, std::function<void(Widget*)> function);
	void trigger(int action);
	void unbind(int action);

	void setLayout(Layout* layout);
	Layout* getLayout() const;

	void setMinimumSize(float width, float height);
	void setMinimumSize(const vec2& size);
	vec2 getMinimumSize() const;

	void setMaximumSize(float width, float height);
	void setMaximumSize(const vec2& size);
    vec2 getMaximumSize() const;

	void setFixedWidth(float width);
	void setFixedHeight(float height);
	void setFixedSize(float width, float height);
	void setFixedSize(const vec2& size);

	void setSize(float width, float height);
	void setSize(const vec2& size);
	vec2 getSize() const;

	void setPosition(float x, float y);
	void setPosition(const vec2& position);
	vec2 getPosition() const;

	sf::RectangleShape getGeometry() const;
	sf::FloatRect getRect() const;

	void setOrigin(const vec2& origin);
	vec2 getOrigin() const;

	void setBackgroundColor(const sf::Color& color);
	sf::Color getBackgroundColor() const;

	void setTexture(const sf::Texture* texture);
	const sf::Texture* getTexture() const { return m_geometry.getTexture(); }

	void setEnabled(bool enabled);
	bool isEnabled() const { return m_isEnabled; }

	void setVisible(bool visible);
	bool isVisible() const { return m_isVisible; }

	bool isFocused() const { return m_isFocused; }
	bool isHovered() const { return m_isHovered; }
	bool isPressed() const { return m_isPressed; }

	unsigned int getId() const { return m_id; }
protected:
	friend class GUI;

	Widget();

	// Вызывается каждый кадр до отрисовки
	virtual void onUpdate() {}

	// Вызывается каждый кадр после обновления всех виджетов
	virtual void onDraw();

	// Вызывается при изменении геометрии
	virtual void update() {}

	Widget* m_parent;

	Layout* m_layout;

	sf::RectangleShape m_geometry;

	vec2 m_minimumSize;
	vec2 m_maximumSize;

	bool m_isEnabled;
	bool m_isVisible;

	bool m_isFocused;
	bool m_isHovered;
	bool m_isPressed;

	std::vector<std::function<void(Widget*)>> m_actions;

	static unsigned int CURRENT_ID;
	unsigned int m_id;
};

typedef std::shared_ptr<Widget> widget_ptr;
