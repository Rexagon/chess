#pragma once

#include <vector>

#include "Widget.h"

class Layout
{
public:
	// ������ �� ����������� �� � ���� ������
	Layout();

	// ������ ������ � ����������� ��� � parent
	Layout(widget_ptr parent);

	virtual ~Layout();

	// ��������� ������ � ��������� ������
	virtual void add_widget(widget_ptr widget);

	// ������� ������ � ��������� ������
	virtual void remove_widget(widget_ptr widget);

	// ���������� ����� ������� ������ �������
	//@ ���� ������ �� �����, ����� -1
	virtual int index_of(widget_ptr widget) const;

	// ���������� ������ ��� ��������� �������
	//@ ���� ����� �� �����, ����� nullptr
	virtual Widget* take_at(int index) const;

    void clear();

	// ������������� ������������ ��� ���� ��������� ������� � ��������� ������
	//@ ���� �� �������� ������������ �� �����-������ �� ����
	//@ �� ��� ���� ��� �� ��������� ����� ������������ �� ������
	void set_alignment(int alignment);

	// ������������� ������������ ��� ���������� �������� � ��������� ������
	//@ ���� ������ �������� ���, �� ������ �� ���������
	//@ ���� �� �������� ������������ �� �����-������ �� ����
	//@ �� ��� ���� ��� �� ��������� ����� ������������ �� ������
	void set_alignment(widget_ptr widget, int alignment);

	// ������������� ������� ����� ���������� � ������� �� ���� �������
	void set_spacing(float spacing) { m_spacing = spacing; }

	// ���������� ������ ������� ����� ���������� � �� ���� �������
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
