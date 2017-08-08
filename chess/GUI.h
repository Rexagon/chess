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

	// Обновляет состояния элементов, нажатия, наведения
	void update(const float dt);

	// Рисует элементы в заданном порядке
	void draw();

	// Обрабатывает ввод текста
	void text_entered(sf::Uint32 text);

	// Возвращает корневой виджет всего окна
	widget_ptr get_root_widget() const { return m_root_widget; }

    void prepare_deleting(Widget* widget);

	// Вспомогательная функция для создания GUI объектов
	template<class T, class... Args>
	static std::shared_ptr<T> create(const Args&... args)
	{
		static_assert(std::is_base_of<Widget, T>::value, "GUI::Create template argument must be the child class of \"Widget\" class");
		std::shared_ptr<T> widget = std::shared_ptr<T>(new T(args...));
		return std::move(widget);
	}
private:
	widget_ptr m_root_widget;

	Widget* m_current_hovered_item;
	Widget* m_current_pressed_item;
	Widget* m_current_focused_item;
};
