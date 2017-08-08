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

	void set_parent(Widget* widget);
	Widget* get_parent() const;

	void bind(int action, std::function<void(Widget*)> function);
	void trigger(int action);
	void unbind(int action);

	void set_layout(Layout* layout);
	Layout* get_layout() const;

	void set_minimum_size(float width, float height);
	void set_minimum_size(const vec2& size);
	vec2 get_minimum_size() const;

	void set_maximum_size(float width, float height);
	void set_maximum_size(const vec2& size);
    vec2 get_maximum_size() const;

	void set_fixed_width(float width);
	void set_fixed_height(float height);
	void set_fixed_size(float width, float height);
	void set_fixed_size(const vec2& size);

	void set_size(float width, float height);
	void set_size(const vec2& size);
	vec2 get_size() const;

	void set_position(float x, float y);
	void set_position(const vec2& position);
	vec2 get_position() const;

	sf::RectangleShape get_geometry() const;
	sf::FloatRect get_rect() const;

	void set_origin(const vec2& origin);
	vec2 get_origin() const;

	void set_background_color(const sf::Color& color);
	sf::Color get_background_color() const;

	void set_outline_color(const sf::Color &color);
	sf::Color get_outline_color() const { return m_geometry.getOutlineColor(); }

	void set_outline_thickness(float thickness);
	float get_outline_thickness() const { return m_geometry.getOutlineThickness(); }

	void set_texture(const sf::Texture* texture);
	const sf::Texture* get_texture() const { return m_geometry.getTexture(); }

	void set_enabled(bool enabled);
	bool is_enabled() const { return m_is_enabled; }

	void set_visible(bool visible);
	bool is_visible() const { return m_is_visible; }

	bool is_focused() const { return m_is_focused; }
	bool is_hovered() const { return m_is_hovered; }
	bool is_pressed() const { return m_is_pressed; }

	unsigned int get_id() const { return m_id; }

	virtual char get_type() const;
protected:
	friend class GUI;

	Widget();

	// Вызывается каждый кадр до отрисовки
	virtual void on_update(const float dt) {}

	// Вызывается каждый кадр после обновления всех виджетов
	virtual void on_draw();

	// Вызывается при изменении геометрии
	virtual void update() {}

	Widget* m_parent;

	Layout* m_layout;

	sf::RectangleShape m_geometry;

	vec2 m_minimum_size;
	vec2 m_maximum_size;

	bool m_is_enabled;
	bool m_is_visible;

	bool m_is_focused;
	bool m_is_hovered;
	bool m_is_pressed;

	std::vector<std::function<void(Widget*)>> m_actions;

	static unsigned int CURRENT_ID;
	unsigned int m_id;
};

typedef std::shared_ptr<Widget> widget_ptr;
