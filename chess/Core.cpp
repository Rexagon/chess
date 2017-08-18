#include "Core.h"

bool Core::m_is_running = false;
sf::RenderWindow Core::m_window;
std::stack<std::unique_ptr<State>> Core::m_states;


void Core::init()
{
	Log::create("log.txt");

	AssetManager::load("data/data.json");
	ConfigManager::init();

	Client::init("95.84.247.9", 49876);

	m_is_running = false;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;
	m_window.create(sf::VideoMode(1280, 800), "Chess club", sf::Style::Default, settings);
	m_window.setVerticalSyncEnabled(true);

	CursorManager::init(m_window.getSystemHandle());
}

void Core::close()
{
	while (!m_states.empty()) {
		delete_state();
	}
	m_window.close();

	AssetManager::clear();
	ConfigManager::close();
	CursorManager::clear();
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
			if (current_state != nullptr) {
				current_state->focus_gained();
			}
			break;


		case sf::Event::LostFocus:
			if (current_state != nullptr) {
				current_state->focus_lost();
			}
			break;


		case sf::Event::Resized:
			if (current_state != nullptr) {
				current_state->resized(static_cast<float>(e.size.width), static_cast<float>(e.size.height));
			}
			break;


		case sf::Event::MouseMoved:
			Input::m_mouse_position = vec2(static_cast<float>(e.mouseMove.x), static_cast<float>(e.mouseMove.y));
			break;


		case sf::Event::KeyPressed:
			if (e.key.code > -1 && e.key.code < sf::Keyboard::KeyCount) {
				Input::m_current_keys_state[e.key.code] = true;
			}
			break;


		case sf::Event::KeyReleased:
			if (e.key.code > -1 && e.key.code < sf::Keyboard::KeyCount) {
				Input::m_current_keys_state[e.key.code] = false;
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
			Input::m_mouse_wheel_delta = static_cast<int>(e.mouseWheelScroll.delta);
			break;
		}

		if (current_state != nullptr) {
			current_state->m_gui.handle_input(e);
		}
	}
}
