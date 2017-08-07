#include "VerticalLayout.h"

#include "Widget.h"
#include "GUI.h"

void VerticalLayout::update()
{
	vec2 size = m_parent->getSize();

	float rowWidth = size.x - m_spacing * 2.0f;
	float rowHeight = (size.y - (m_items.size() + 1.0f) * m_spacing) / m_items.size();
	vec2 rowSize(rowWidth, rowHeight);

	for (unsigned int i = 0; i < m_items.size(); ++i) {
		Item& item = m_items[i];
		item.widget->setSize(rowSize);
		vec2 itemSize = item.widget->getSize();

		vec2 itemPosition;
		if (item.alignment & GUI::AlignLeft) {
			itemPosition.x = m_spacing;
		}
		else if (item.alignment & GUI::AlignRight) {
			itemPosition.x = size.x - m_spacing - itemSize.x;
		}
		else if (item.alignment & GUI::AlignHCenter) {
			itemPosition.x = (size.x - itemSize.x) / 2.0f;
		}

		itemPosition.y = (m_spacing + rowHeight) * static_cast<float>(i);
		if (item.alignment & GUI::AlignTop) {
			itemPosition.y += m_spacing;
		}
		else if (item.alignment & GUI::AlignBottom) {
			itemPosition.y += m_spacing + rowHeight - itemSize.y;
		}
		else if (item.alignment & GUI::AlignVCenter) {
			itemPosition.y += m_spacing + (rowHeight - itemSize.y) / 2.0f;
		}

		item.widget->setPosition(m_parent->getPosition() + itemPosition);
	}
}
