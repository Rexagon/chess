#include "VerticalLayout.h"

#include "Widget.h"
#include "GUI.h"

void VerticalLayout::update()
{
	vec2 size = m_parent->get_size();

	float row_width = size.x - m_spacing * 2.0f;
	float row_height = (size.y - (m_items.size() + 1.0f) * m_spacing) / m_items.size();

	for (unsigned int i = 0; i < m_items.size(); ++i) {
		Item& item = m_items[i];
		item.widget->set_size(row_width, row_height);
		vec2 item_size = item.widget->get_size();

		vec2 item_position;
		if (item.alignment & GUI::AlignLeft) {
			item_position.x = m_spacing;
		}
		else if (item.alignment & GUI::AlignRight) {
			item_position.x = size.x - m_spacing - item_size.x;
		}
		else if (item.alignment & GUI::AlignHCenter) {
			item_position.x = (size.x - item_size.x) / 2.0f;
		}

		item_position.y = (m_spacing + row_height) * static_cast<float>(i);
		if (item.alignment & GUI::AlignTop) {
			item_position.y += m_spacing;
		}
		else if (item.alignment & GUI::AlignBottom) {
			item_position.y += m_spacing + row_height - item_size.y;
		}
		else if (item.alignment & GUI::AlignVCenter) {
			item_position.y += m_spacing + (row_height - item_size.y) / 2.0f;
		}

		item.widget->set_position(m_parent->get_position() + item_position);
	}
}
