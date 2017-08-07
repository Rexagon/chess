#include "HorizontalLayout.h"

#include "Widget.h"
#include "GUI.h"

void HorizontalLayout::update()
{
	vec2 size = m_parent->getSize();

	float colWidth = (size.x - (m_items.size() + 1.0f) * m_spacing) / m_items.size();
	float colHeight = size.y - m_spacing * 2.0f;
	vec2 rowSize(colWidth, colHeight);

	for (unsigned int i = 0; i < m_items.size(); ++i) {
		Item& item = m_items[i];
		item.widget->setSize(rowSize);
		vec2 itemSize = item.widget->getSize();

		vec2 itemPosition;
		itemPosition.x = (m_spacing + colWidth) * static_cast<float>(i);
		if (item.alignment & GUI::AlignLeft) {
			itemPosition.x += m_spacing;
		}
		else if (item.alignment & GUI::AlignRight) {
			itemPosition.x += m_spacing + colWidth - itemSize.x;
		}
		else if (item.alignment & GUI::AlignHCenter) {
			itemPosition.x += m_spacing + (colWidth - itemSize.x) / 2.0f;
		}

		if (item.alignment & GUI::AlignTop) {
			itemPosition.y = m_spacing;
		}
		else if (item.alignment & GUI::AlignBottom) {
			itemPosition.y = size.y - m_spacing - itemSize.y;
		}
		else if (item.alignment & GUI::AlignVCenter) {
			itemPosition.y = (size.y - itemSize.y) / 2.0f;
		}

		item.widget->setPosition(m_parent->getPosition() + itemPosition);
	}
}
