#include "Core.h"

bool Core::m_is_running = false;
sf::RenderWindow Core::m_window;
std::stack<std::unique_ptr<State>> Core::m_states;


void Core::init()
{
	Log::create("log.txt");

	AssetManager::load("data/data.json");

	m_is_running = false;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;
	m_window.create(sf::VideoMode(1024, 600), "CYBR", sf::Style::Default, settings);
	m_window.setVerticalSyncEnabled(true);
}

void Core::close()
{
	while (!m_states.empty()) {
		delete_state();
	}
	m_window.close();

	AssetManager::clear();
}

void Core::run()
{
	m_is_running = true;

	sf::Clock timer;
	while (m_is_running && !m_states.empty())
	{
		float dt = timer.restart().asSeconds();
	
		handle_input(dt);

		State* currentState = nullptr;

		if (currentState = get_state()) {
			currentState->m_gui.update(dt);
		}

		if (currentState = get_state()) {
			currentState->update(dt);
		}

		m_window.clear(sf::Color(60, 60, 60));

		if (currentState = get_state()) {
			currentState->draw(dt);
		}

		m_window.display();
	}
}

void Core::stop()
{
	m_is_running = false;
}

void Core::draw(const sf::Drawable & drawable, const sf::RenderStates& states)
{
	m_window.draw(drawable, states);
}

void Core::draw(const sf::Drawable & drawable, sf::Shader * shader)
{
	sf::RenderStates states;
	states.shader = shader;
	m_window.draw(drawable, states);
}

void Core::draw(const sf::Vertex* vertices, std::size_t vertex_count,
	sf::PrimitiveType type, const sf::RenderStates& states)
{
	m_window.draw(vertices, vertex_count, type, states);
}

void Core::handle_input(const float dt)
{
	Input::update();

	sf::Event e;

	while (m_window.pollEvent(e)) {
		int value = 0;
		State* current_state = get_state();

		switch (e.type) {
		case sf::Event::Closed:
			Core::stop();
			break;
		case sf::Event::GainedFocus:
			if (current_state) {
				current_state->focus_gained();
			}
			break;
		case sf::Event::LostFocus:
			if (current_state) {
				current_state->focus_lost();
			}
			break;
		case sf::Event::Resized:
			if (current_state) {
				current_state->resized(static_cast<float>(e.size.width), static_cast<float>(e.size.height));
			}
		case sf::Event::MouseMoved:
			Input::m_mouse_position = vec2(static_cast<float>(e.mouseMove.x), static_cast<float>(e.mouseMove.y));
			break;
		case sf::Event::KeyPressed:
			if (e.key.code > -1 && e.key.code < sf::Keyboard::KeyCount) {
				Input::m_current_keys_state[e.key.code] = true;

				if (e.key.code >= Key::Left && e.key.code <= Key::Down) {
					current_state->m_gui.text_entered(e.key.code - 70); // 71-74 to ascii 25-28
				}
			}
			break;
		case sf::Event::KeyReleased:
			if (e.key.code > -1 && e.key.code < sf::Keyboard::KeyCount) {
				Input::m_current_keys_state[e.key.code] = false;
			}
			break;
		case sf::Event::TextEntered:
			if (current_state) {
				current_state->m_gui.text_entered(e.text.unicode);
			}
			break;
		case sf::Event::MouseButtonPressed:
			if (e.mouseButton.button > -1 && e.mouseButton.button < sf::Mouse::ButtonCount) {
				Input::m_current_mouse_buttons_state[e.mouseButton.button] = true;
			}
			break;
		case sf::Event::MouseButtonReleased:
			if (e.mouseButton.button > -1 && e.mouseButton.button < sf::Mouse::ButtonCount) {
				Input::m_current_mouse_buttons_state[e.mouseButton.button] = false;
			}
			break;
		case sf::Event::MouseWheelScrolled:
			Input::m_mouse_wheel_delta = e.mouseWheelScroll.delta;
		default:
			break;
		}
	}
}
