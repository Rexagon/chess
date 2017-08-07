#include "GUI.h"

#include <stack>

#include "Core.h"

GUI::GUI() :
    m_currentFocusedItem(nullptr), m_currentHoveredItem(nullptr), m_currentPressedItem(nullptr)
{
	sf::Vector2u windowSize = Core::getWindow()->getSize();

	m_rootWidget = std::shared_ptr<Widget>(new Widget);
	m_rootWidget->setPosition(0.0f, 0.0f);
	m_rootWidget->setSize(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));
	m_rootWidget->setLayout(new Layout);
}

void GUI::update()
{
	vec2 mousePosition = Input::getMousePosition();

	Widget* hoveredItem = nullptr;
	int hoveredItemIndex = -1;

	std::stack<Widget*> widgets;
	widgets.push(m_rootWidget.get());

	while (!widgets.empty()) {
		Widget* widget = widgets.top();
		widgets.pop();

		if (widget->m_layout) {
			std::vector<widget_ptr>& children = widget->m_layout->m_orderedWidgets;

			// проверка наведения
			int itemIndex = -1;
			for (unsigned int i = 0; i < children.size(); ++i) {
				if (i == 0) {
					widget->m_layout->update();
				}
				if (children[i]->getRect().contains(mousePosition.x, mousePosition.y)) {
					itemIndex = i;
					hoveredItem = children[i].get();
				}
			}

			if (itemIndex > -1) {
				hoveredItemIndex = itemIndex;
			}

			// обновление
			for (auto& child : children) {
				child->onUpdate();
				widgets.push(child.get());
			}
		}
	}

	// при изменеии наведённого элемента
    if (hoveredItem != m_currentHoveredItem) {
		if (m_currentHoveredItem != nullptr) {
			m_currentHoveredItem->m_isHovered = false;
			m_currentHoveredItem->trigger(Widget::Action::Unhover);
			m_currentHoveredItem = nullptr;
		}

        if (hoveredItem != nullptr) {
			m_currentHoveredItem = hoveredItem;
			m_currentHoveredItem->m_isHovered = true;
			m_currentHoveredItem->trigger(Widget::Action::Hover);
		}
	}

	// обработка нажатия
	if (m_currentHoveredItem != nullptr && Input::getMouseDown(MouseButton::Left)) {
		// двигаем нажатый элемент наверх по Z
		if (hoveredItemIndex > -1) {
			auto& orderedWidgets = m_currentHoveredItem->getParent()->m_layout->m_orderedWidgets;
			std::rotate(orderedWidgets.begin() + hoveredItemIndex, orderedWidgets.begin() + hoveredItemIndex + 1,
						orderedWidgets.end());
		}

		m_currentHoveredItem->m_isPressed = true;
		m_currentHoveredItem->trigger(Widget::Action::Press);
		m_currentPressedItem = m_currentHoveredItem;
	}

	if (m_currentPressedItem != nullptr && Input::getMouseUp(MouseButton::Left)) {
		m_currentPressedItem->m_isPressed = false;
		m_currentPressedItem->trigger(Widget::Action::Release);
		m_currentPressedItem = nullptr;
	}
}

void GUI::draw()
{
	std::stack<Widget*> widgets;
	widgets.push(m_rootWidget.get());

	while (!widgets.empty()) {
		Widget* widget = widgets.top();
		widgets.pop();

		if (widget->m_layout) {
			std::vector<widget_ptr>& children = widget->m_layout->m_orderedWidgets;

			for (auto& child : children) {
				child->onDraw();
				widgets.push(child.get());
			}
		}
    }
}

void GUI::prepareDeleting(Widget *widget)
{
    if (widget == m_currentFocusedItem) {
        m_currentFocusedItem = nullptr;
    }
    if (widget == m_currentHoveredItem) {
        m_currentHoveredItem = nullptr;
    }
    if (widget == m_currentPressedItem) {
        m_currentPressedItem = nullptr;
    }
}
