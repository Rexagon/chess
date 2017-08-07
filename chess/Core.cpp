#include "Core.h"

bool Core::m_isRunning = false;
sf::RenderWindow Core::m_window;
std::stack<std::unique_ptr<State>> Core::m_states;


void Core::init()
{
	Log::create("log.txt");

	AssetManager::load("data/data.json");

	m_isRunning = false;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;
	m_window.create(sf::VideoMode(1024, 600), "CYBR", sf::Style::Default, settings);
	m_window.setVerticalSyncEnabled(true);
}

void Core::close()
{
	while (!m_states.empty()) {
		deleteState();
	}
	m_window.close();

	AssetManager::clear();
}

void Core::run()
{
	m_isRunning = true;

	sf::Clock timer;
	while (m_isRunning && !m_states.empty())
	{
		float dt = timer.restart().asSeconds();
	
		HandleInput(dt);

		State* currentState = nullptr;

		if (currentState = getState()) {
			currentState->m_gui.update();
		}

		if (currentState = getState()) {
			currentState->update(dt);
		}

		m_window.clear(sf::Color(60, 60, 60));

		if (currentState = getState()) {
			currentState->draw(dt);
		}

		m_window.display();
	}
}

void Core::stop()
{
	m_isRunning = false;
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

void Core::draw(const sf::Vertex* vertices, std::size_t vertexCount,
	sf::PrimitiveType type, const sf::RenderStates& states)
{
	m_window.draw(vertices, vertexCount, type, states);
}

void Core::HandleInput(const float dt)
{
	Input::update();

	sf::Event e;

	while (m_window.pollEvent(e)) {
		int value = 0;
		State* currentState = getState();

		switch (e.type) {
		case sf::Event::Closed:
			Core::stop();
			break;
		case sf::Event::GainedFocus:
			if (currentState) {
				currentState->focusGained();
			}
			break;
		case sf::Event::LostFocus:
			if (currentState) {
				currentState->focusLost();
			}
			break;
		case sf::Event::MouseMoved:
			Input::m_mousePosition = vec2(static_cast<float>(e.mouseMove.x), static_cast<float>(e.mouseMove.y));
			break;
		case sf::Event::KeyPressed:
			if (e.key.code > -1 && e.key.code < sf::Keyboard::KeyCount) {
				Input::m_currentKeysState[e.key.code] = true;
			}
			break;
		case sf::Event::KeyReleased:
			if (e.key.code > -1 && e.key.code < sf::Keyboard::KeyCount) {
				Input::m_currentKeysState[e.key.code] = false;
			}
			break;
		case sf::Event::MouseButtonPressed:
			if (e.mouseButton.button > -1 && e.mouseButton.button < sf::Mouse::ButtonCount) {
				Input::m_currentMouseButtonsState[e.mouseButton.button] = true;
			}
			break;
		case sf::Event::MouseButtonReleased:
			if (e.mouseButton.button > -1 && e.mouseButton.button < sf::Mouse::ButtonCount) {
				Input::m_currentMouseButtonsState[e.mouseButton.button] = false;
			}
			break;
		case sf::Event::MouseWheelScrolled:
			Input::m_mouseWheelDelta = e.mouseWheelScroll.delta;
		default:
			break;
		}
	}
}
