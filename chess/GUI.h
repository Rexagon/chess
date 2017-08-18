#pragma once

#include <memory>
#include <vector>
#include <string>
#include <map>

#include "Layout.h"
#include "VerticalLayout.h"
#include "HorizontalLayout.h"

#include "Widget.h"
#include "Label.h"
#include "TextBox.h"

class GUI
{
public:
	enum Alignment
	{
		AlignLeft = 1 << 0,
		AlignRight = 1 << 1,
		AlignHCenter = 1 << 2,

		AlignTop = 1 << 3,
		AlignBottom = 1 << 4,
		AlignVCenter = 1 << 5,

		AlignCenter = AlignHCenter | AlignVCenter
	};

	enum WidgetType
	{
		EmptyWidget,
		LabelWidget,
		TextBoxWidget
	};

	GUI();
	~GUI();

	template<class T, class... Args>
	std::shared_ptr<T> create(const Args&... args)
	{
		static_assert(std::is_base_of<Widget, T>::value, "GUI::Create template argument must be the child class of \"Widget\" class");
		std::shared_ptr<T> widget = std::make_shared<T>(args...);
		widget->m_gui = this;
		return std::move(widget);
	}

	// Обновляет состояния элементов, нажатия, наведения
	void update(const float dt);

	void handle_input(const sf::Event& event);

	// Рисует элементы в заданном порядке
	void draw();

	std::shared_ptr<Widget> get_element(const vec2& screen_position);

	void press_item(std::shared_ptr<Widget> item);
	void focus_item(std::shared_ptr<Widget> item);
	void hover_item(std::shared_ptr<Widget> item);

	// Возвращает корневой виджет всего окна
	std::shared_ptr<Widget> get_root_widget() const { return m_root_widget; }

	Widget* get_hovered_item() { return m_current_hovered_item.get(); }
	Widget* get_pressed_item() { return m_current_pressed_item.get(); }
	Widget* get_focused_item() { return m_current_focused_item.get(); }

    void prepare_deleting(Widget* widget);
private:
	std::shared_ptr<Widget> m_root_widget;

	std::shared_ptr<Widget> m_current_hovered_item;
	std::shared_ptr<Widget> m_current_pressed_item;
	std::shared_ptr<Widget> m_current_focused_item;
};
