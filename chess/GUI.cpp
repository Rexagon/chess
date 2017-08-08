#include "GUI.h"

#include <stack>

#include "Core.h"

GUI::GUI() :
    m_current_focused_item(nullptr), m_current_hovered_item(nullptr), m_current_pressed_item(nullptr)
{
	sf::Vector2u windowSize = Core::get_window()->getSize();

	m_root_widget = std::shared_ptr<Widget>(new Widget);
	m_root_widget->set_position(0.0f, 0.0f);
	m_root_widget->set_size(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));
	m_root_widget->set_layout(new Layout);
}

void GUI::update(const float dt)
{
	vec2 mouse_position = Input::get_mouse_position();

	Widget* hovered_item = nullptr;
	int hovered_item_index = -1;

	std::stack<Widget*> widgets;
	widgets.push(m_root_widget.get());

	while (!widgets.empty()) {
		Widget* widget = widgets.top();
		widgets.pop();

		if (widget->m_layout) {
			std::vector<widget_ptr>& children = widget->m_layout->m_ordered_widgets;

			// проверка наведения
			int itemIndex = -1;
			for (unsigned int i = 0; i < children.size(); ++i) {
				if (i == 0) {
					widget->m_layout->update();
				}
				if (children[i]->get_rect().contains(mouse_position.x, mouse_position.y) &&
					children[i]->is_visible()) {
					itemIndex = i;
					hovered_item = children[i].get();
				}
			}

			if (itemIndex > -1) {
				hovered_item_index = itemIndex;
			}

			// обновление
			for (auto& child : children) {
				child->on_update(dt);
				widgets.push(child.get());
			}
		}
	}

	// при изменеии наведённого элемента
    if (hovered_item != m_current_hovered_item) {
		if (m_current_hovered_item != nullptr) {
			m_current_hovered_item->m_is_hovered = false;
			m_current_hovered_item->trigger(Widget::Action::Unhover);
			m_current_hovered_item = nullptr;
		}

        if (hovered_item != nullptr) {
			m_current_hovered_item = hovered_item;
			m_current_hovered_item->m_is_hovered = true;
			m_current_hovered_item->trigger(Widget::Action::Hover);
		}
	}

	// обработка нажатия
	if (m_current_hovered_item != nullptr && Input::get_mouse_down(MouseButton::Left) &&
		m_current_hovered_item->is_enabled()) 
	{
		// двигаем нажатый элемент наверх по Z
		if (hovered_item_index > -1) {
			auto& orderedWidgets = m_current_hovered_item->get_parent()->m_layout->m_ordered_widgets;
			std::rotate(orderedWidgets.begin() + hovered_item_index, orderedWidgets.begin() + hovered_item_index + 1,
						orderedWidgets.end());
		}

		m_current_hovered_item->m_is_pressed = true;
		m_current_hovered_item->trigger(Widget::Action::Press);
		m_current_pressed_item = m_current_hovered_item;

		if (m_current_focused_item != m_current_hovered_item) {
			if (m_current_focused_item != nullptr) {
				m_current_focused_item->trigger(Widget::Action::Unfocus);
				m_current_focused_item->m_is_focused = false;
			}
			m_current_hovered_item->trigger(Widget::Action::Focus);
			m_current_hovered_item->m_is_focused = true;
			m_current_focused_item = m_current_hovered_item;
		}
	}

	if (m_current_pressed_item != nullptr && Input::get_mouse_up(MouseButton::Left)) {
		m_current_pressed_item->m_is_pressed = false;
		m_current_pressed_item->trigger(Widget::Action::Release);
		m_current_pressed_item = nullptr;
	}
}

void GUI::draw()
{
	std::stack<Widget*> widgets;
	widgets.push(m_root_widget.get());

	while (!widgets.empty()) {
		Widget* widget = widgets.top();
		widgets.pop();

		if (widget->m_layout) {
			std::vector<widget_ptr>& children = widget->m_layout->m_ordered_widgets;

			for (auto& child : children) {
				child->on_draw();
				widgets.push(child.get());
			}
		}
    }
}

void GUI::text_entered(sf::Uint32 text)
{
	if (m_current_focused_item != nullptr && 
		m_current_focused_item->get_type() == WidgetType::TextBoxWidget) 
	{
		TextBox* label = reinterpret_cast<TextBox*>(m_current_focused_item);
		label->handle_text_enter(text);
	}
}

void GUI::prepare_deleting(Widget *widget)
{
    if (widget == m_current_focused_item) {
        m_current_focused_item = nullptr;
    }
    if (widget == m_current_hovered_item) {
        m_current_hovered_item = nullptr;
    }
    if (widget == m_current_pressed_item) {
        m_current_pressed_item = nullptr;
    }
}
