#include "Layout.h"

#include "GUI.h"
#include "Log.h"

Layout::Layout() :
    m_parent(nullptr), m_spacing(0.0f)
{
}

Layout::Layout(std::shared_ptr<Widget> parent) :
	m_parent(parent.get())
{
	parent->set_layout(this);
}

Layout::~Layout()
{
    m_items.clear();
	m_ordered_widgets.clear();
}

void Layout::add_widget(std::shared_ptr<Widget> widget)
{
	widget->set_parent(m_parent);
	m_ordered_widgets.push_back(widget);
	m_items.push_back(Item(widget.get(), GUI::AlignCenter));
	update();
}

void Layout::remove_widget(std::shared_ptr<Widget> widget)
{
	for (auto it = m_items.begin(); it != m_items.end(); ++it) {
		if ((*it).widget == widget.get()) {
			m_items.erase(it);
			break;
		}
	}
	for (auto it = m_ordered_widgets.begin(); it != m_ordered_widgets.end(); ++it) {
		if ((*it) == widget) {
			m_ordered_widgets.erase(it);
			break;
		}
	}
	update();
}

int Layout::index_of(std::shared_ptr<Widget> widget) const
{
	for (unsigned int i = 0; i < m_items.size(); ++i) {
		if (m_items[i].widget == widget.get()) {
			return i;
		}
	}
	return -1;
}

Widget * Layout::take_at(int index) const
{
	if (index > -1 && index < m_items.size()) {
		return m_items[index].widget;
	}
    return nullptr;
}

void Layout::clear()
{
    m_items.clear();
    m_ordered_widgets.clear();
}

void Layout::set_alignment(int alignment)
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

void Layout::set_alignment(std::shared_ptr<Widget> widget, int alignment)
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
