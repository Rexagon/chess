#pragma once

#include <vector>

#include "Widget.h"

class Layout
{
public:
	// ������ �� ����������� �� � ���� ������
	Layout();

	// ������ ������ � ����������� ��� � parent
	Layout(std::shared_ptr<Widget> parent);

	virtual ~Layout();

	// ��������� ������ � ��������� ������
	virtual void add_widget(std::shared_ptr<Widget> widget);

	// ������� ������ � ��������� ������
	virtual void remove_widget(std::shared_ptr<Widget> widget);

	// ���������� ����� ������� ������ �������
	//@ ���� ������ �� �����, ����� -1
	virtual int index_of(std::shared_ptr<Widget> widget) const;

	// ���������� ������ ��� ��������� �������
	//@ ���� ����� �� ������, ����� nullptr
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
	void set_alignment(std::shared_ptr<Widget> widget, int alignment);

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
	std::vector<std::shared_ptr<Widget>> m_ordered_widgets;
};
