#include "HorizontalLayout.h"

#include "Widget.h"
#include "GUI.h"

void HorizontalLayout::update()
{
	vec2 size = m_parent->get_size();

	float column_width = (size.x - (m_items.size() + 1.0f) * m_spacing) / m_items.size();
	float column_height = size.y - m_spacing * 2.0f;
	vec2 rowSize(column_width, column_height);

	for (unsigned int i = 0; i < m_items.size(); ++i) {
		Item& item = m_items[i];
		item.widget->set_size(rowSize);
		vec2 item_size = item.widget->get_size();

		vec2 item_position;
		item_position.x = (m_spacing + column_width) * static_cast<float>(i);
		if (item.alignment & GUI::AlignLeft) {
			item_position.x += m_spacing;
		}
		else if (item.alignment & GUI::AlignRight) {
			item_position.x += m_spacing + column_width - item_size.x;
		}
		else if (item.alignment & GUI::AlignHCenter) {
			item_position.x += m_spacing + (column_width - item_size.x) / 2.0f;
		}

		if (item.alignment & GUI::AlignTop) {
			item_position.y = m_spacing;
		}
		else if (item.alignment & GUI::AlignBottom) {
			item_position.y = size.y - m_spacing - item_size.y;
		}
		else if (item.alignment & GUI::AlignVCenter) {
			item_position.y = (size.y - item_size.y) / 2.0f;
		}

		item.widget->set_position(m_parent->get_position() + item_position);
	}
}
