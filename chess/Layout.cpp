#include "Layout.h"

#include "GUI.h"
#include "Log.h"
Layout::Layout() :
    m_parent(nullptr), m_spacing(0.0f)
{
}

Layout::Layout(widget_ptr parent) :
	m_parent(parent.get())
{
	parent->setLayout(this);
}

Layout::~Layout()
{
    m_items.clear();
	m_orderedWidgets.clear();
}

void Layout::addWidget(widget_ptr widget)
{
	widget->setParent(m_parent);
	m_orderedWidgets.push_back(widget);
	m_items.push_back(Item(widget.get(), GUI::AlignCenter));
	update();
}

void Layout::removeWidget(widget_ptr widget)
{
	for (auto it = m_items.begin(); it != m_items.end(); ++it) {
		if ((*it).widget == widget.get()) {
			m_items.erase(it);
			break;
		}
	}
	for (auto it = m_orderedWidgets.begin(); it != m_orderedWidgets.end(); ++it) {
		if ((*it) == widget) {
			m_orderedWidgets.erase(it);
			break;
		}
	}
	update();
}

int Layout::indexOf(widget_ptr widget) const
{
	for (unsigned int i = 0; i < m_items.size(); ++i) {
		if (m_items[i].widget == widget.get()) {
			return i;
		}
	}
	return -1;
}

Widget * Layout::takeAt(int index) const
{
	if (index > -1 && index < m_items.size()) {
		return m_items[index].widget;
	}
    return nullptr;
}

void Layout::clear()
{
    m_items.clear();
    m_orderedWidgets.clear();
}

void Layout::setAlignment(int alignment)
{
	if (!(alignment & GUI::AlignLeft ||
		alignment & GUI::AlignRight ||
		alignment & GUI::AlignHCenter)) {
		alignment |= GUI::AlignHCenter;
	}

	if (!(alignment & GUI::AlignTop ||
		alignment & GUI::AlignBottom ||
		alignment & GUI::AlignVCenter)) {
		alignment |= GUI::AlignVCenter;
	}

	for (unsigned int i = 0; i < m_items.size(); ++i) {
		m_items[i].alignment = alignment;
	}
	update();
}

void Layout::setAlignment(widget_ptr widget, int alignment)
{
	if (!(alignment & GUI::AlignLeft ||
		alignment & GUI::AlignRight ||
		alignment & GUI::AlignHCenter)) {
		alignment |= GUI::AlignHCenter;
	}

	if (!(alignment & GUI::AlignTop ||
		alignment & GUI::AlignBottom ||
		alignment & GUI::AlignVCenter)) {
		alignment |= GUI::AlignVCenter;
	}

	for (unsigned int i = 0; i < m_items.size(); ++i) {
		if (widget == nullptr || m_items[i].widget == widget.get()) {
			m_items[i].alignment = alignment;
		}
	}
	update();
}

void Layout::update()
{
}
