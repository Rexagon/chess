#pragma once

#include <vector>

#include "Widget.h"

class Layout
{
public:
	// Создаёт не привязанный ни к кому лэйаут
	Layout();

	// Создаёт лжйаут и привязывает его к parent
	Layout(widget_ptr parent);

	virtual ~Layout();

	// Добавляет виджет и обновляет лэйаут
	virtual void add_widget(widget_ptr widget);

	// Удаляет виджет и обновляет лэйаут
	virtual void remove_widget(widget_ptr widget);

	// Возвращает номер виджета внутри лэйаута
	//@ если виджет не найдён, вернёт -1
	virtual int index_of(widget_ptr widget) const;

	// Возвращает виджет под указанным номером
	//@ если такой не найдёт, вернёт nullptr
	virtual Widget* take_at(int index) const;

    void clear();

	// Устанавливает выравнивание для всех элементов лэйаута и обновляет лэйаут
	//@ если не указанно выравнивание по какой-нибудь из осей
	//@ то для этой оси по умолчанию будет выравнивание по центру
	void set_alignment(int alignment);

	// Устанавливает выравнивание для указанного элемента и обновляет лэйаут
	//@ если такого элемента нет, то ничего не произойдёт
	//@ если не указанно выравнивание по какой-нибудь из осей
	//@ то для этой оси по умолчанию будет выравнивание по центру
	void set_alignment(widget_ptr widget, int alignment);

	// Устанавливает отступы между элементами и отступы от краёв лэйаута
	void set_spacing(float spacing) { m_spacing = spacing; }

	// Возвращает размер отступа между элементами и от краёв лэйаута
	float get_spacing() const { return m_spacing; }
protected:
	friend class GUI;
	friend class Widget;

	virtual void update();

	Widget* m_parent;

	float m_spacing;

	struct Item
	{
		Item(Widget* widget, int alignment) :
			widget(widget), alignment(alignment) {}

		Widget* widget;
		int alignment;
	};
	std::vector<Item> m_items;
	std::vector<widget_ptr> m_ordered_widgets;
};
