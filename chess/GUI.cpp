#include "GUI.h"

#include <stack>

#include "Core.h"

GUI::GUI() :
    m_current_focused_item(nullptr), m_current_hovered_item(nullptr), m_current_pressed_item(nullptr)
{
	sf::Vector2u windowSize = Core::get_window()->getSize();

	m_root_widget = create<Widget>();
	m_root_widget->set_position(0.0f, 0.0f);
	m_root_widget->set_size(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));
	m_root_widget->set_layout(new Layout);
}

GUI::~GUI()
{
	m_current_focused_item.reset();
	m_current_hovered_item.reset();
	m_current_pressed_item.reset();

	m_root_widget.reset();
}

void GUI::update(const float dt)
{
	std::shared_ptr<Widget> hovered_item;

	std::stack<Widget*> widgets;
	widgets.push(m_root_widget.get());

	while (!widgets.empty()) {
		Widget* widget = widgets.top();
		widgets.pop();

		if (widget != nullptr && widget->m_layout != nullptr) {
			for (unsigned int i = 0; i < widget->m_layout->m_ordered_widgets.size(); ++i) {
				auto& child = widget->m_layout->m_ordered_widgets[i];

				if (i == 0) {
					widget->m_layout->update();
				}

				if (child != nullptr) {
					if (child->get_rect().contains(Input::get_mouse_position()) &&
						child->is_visible())
					{
						hovered_item = child;
					}

					child->on_update(dt);
					widgets.push(child.get());
				}
			}
		}
	}

	hover_item(hovered_item);


	// обработка нажатия
	if (Input::get_mouse_down(MouseButton::Left) &&
		m_current_hovered_item != nullptr && m_current_hovered_item->is_enabled())
	{
		// двигаем нажатый элемент наверх по Z
		if (m_current_hovered_item->m_parent != nullptr && m_current_hovered_item->m_parent->m_layout != nullptr) {
			int item_index = m_current_hovered_item->m_parent->m_layout->index_of(m_current_hovered_item);
			if (item_index > -1) {
				auto& orderedWidgets = m_current_hovered_item->get_parent()->m_layout->m_ordered_widgets;
				std::rotate(orderedWidgets.begin() + item_index, orderedWidgets.begin() + item_index + 1, orderedWidgets.end());
			}
		}

		press_item(m_current_hovered_item);
		focus_item(m_current_pressed_item);
	}

	if (Input::get_mouse_up(MouseButton::Left) &&
		m_current_pressed_item != nullptr) 
	{
		press_item(nullptr);
	}

	if (Input::get_key_down(Key::Return)) {
		press_item(m_current_focused_item);
	}
	else if (Input::get_key_up(Key::Return)) {
		press_item(nullptr);
	}

}

void GUI::handle_input(const sf::Event & event)
{
	TextBox* current_textbox = nullptr;

	if (m_current_focused_item != nullptr &&
		m_current_focused_item->get_type() == WidgetType::TextBoxWidget)
	{
		current_textbox = reinterpret_cast<TextBox*>(m_current_focused_item.get());
	}

	switch (event.type) {
	case sf::Event::KeyPressed:
		if (current_textbox != nullptr &&
			event.key.code >= Key::Left && event.key.code <= Key::Down)
		{
			current_textbox->handle_text_enter(event.key.code - 70); // 71-74 to ascii 1-4
		}
		break;


	case sf::Event::TextEntered:
		if (current_textbox != nullptr)
		{
			current_textbox->handle_text_enter(event.text.unicode);
		}
		break;
	}
}

void GUI::draw()
{
	std::stack<Widget*> widgets;
	widgets.push(m_root_widget.get());

	while (!widgets.empty()) {
		Widget* widget = widgets.top();
		widgets.pop();

		if (widget != nullptr && widget->m_layout != nullptr) {
			for (auto& child : widget->m_layout->m_ordered_widgets) {
				if (child != nullptr) {
					child->on_draw();
					widgets.push(child.get());
				}
			}
		}
    }
}

std::shared_ptr<Widget> GUI::get_element(const vec2 & screen_position)
{
	std::shared_ptr<Widget> hovered_item;

	std::stack<Widget*> widgets;
	widgets.push(m_root_widget.get());

	while (!widgets.empty()) {
		Widget* widget = widgets.top();
		widgets.pop();

		if (widget != nullptr && widget->m_layout != nullptr) {
			for (auto& child : widget->m_layout->m_ordered_widgets) {
				if (child != nullptr) {
					if (child->get_rect().contains(Input::get_mouse_position()) &&
						child->is_visible())
					{
						hovered_item = child;
					}

					widgets.push(child.get());
				}
			}
		}
	}

	return hovered_item;
}

void GUI::prepare_deleting(Widget* widget)
{
    if (widget == m_current_focused_item.get()) {
        m_current_focused_item.reset();
    }
    if (widget == m_current_hovered_item.get()) {
        m_current_hovered_item.reset();
    }
    if (widget == m_current_pressed_item.get()) {
        m_current_pressed_item.reset();
    }
}

void GUI::press_item(std::shared_ptr<Widget> item)
{
	if (m_current_pressed_item == item) return;

	if (m_current_pressed_item != nullptr) {
		m_current_pressed_item->m_is_pressed = false;
		m_current_pressed_item->trigger(Widget::Action::Release);
	}

	if (item != nullptr) {
		item->m_is_pressed = true;
		item->trigger(Widget::Action::Press);
	}

	m_current_pressed_item = item;
}

void GUI::focus_item(std::shared_ptr<Widget> item)
{
	if (m_current_focused_item == item) return;

	if (m_current_focused_item != nullptr) {
		m_current_focused_item->trigger(Widget::Action::Unfocus);
		m_current_focused_item->m_is_focused = false;
	}

	if (item != nullptr) {
		item->trigger(Widget::Action::Focus);
		item->m_is_focused = true;
	}

	m_current_focused_item = item;
}

void GUI::hover_item(std::shared_ptr<Widget> item)
{
	if (m_current_hovered_item == item) return;

	if (m_current_hovered_item != nullptr) {
		m_current_hovered_item->trigger(Widget::Action::Unhover);
		m_current_hovered_item->m_is_hovered = false;
	}

	if (item != nullptr) {
		item->trigger(Widget::Action::Hover);
		item->m_is_hovered = true;
	}

	m_current_hovered_item = item;
}
