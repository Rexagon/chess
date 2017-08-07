#include "Input.h"

#include "Core.h"

const int Input::NUM_KEYS = sf::Keyboard::KeyCount;
const int Input::NUM_MOUSEBUTTONS = sf::Mouse::ButtonCount;

vec2 Input::m_mousePosition = vec2(0, 0);
int Input::m_mouseWheelDelta = 0;

std::vector<bool> Input::m_currentKeysState = std::vector<bool>(NUM_KEYS, false);
std::vector<bool> Input::m_lastKeysState = std::vector<bool>(NUM_KEYS, false);

std::vector<bool> Input::m_currentMouseButtonsState = std::vector<bool>(NUM_MOUSEBUTTONS, false);
std::vector<bool> Input::m_lastMouseButtonsState = std::vector<bool>(NUM_MOUSEBUTTONS, false);


void Input::update()
{
	m_lastKeysState = m_currentKeysState;
	m_lastMouseButtonsState = m_currentMouseButtonsState;
	m_mouseWheelDelta = 0;
}

bool Input::getKey(Key keyCode)
{
	if (keyCode < 0) return false;
	return m_currentKeysState[keyCode];
}

bool Input::getKeyDown(Key keyCode)
{
	if (keyCode < 0) return false;
	return m_currentKeysState[keyCode] &&
		!m_lastKeysState[keyCode];
}

bool Input::getKeyUp(Key keyCode)
{
	if (keyCode < 0) return false;
	return !m_currentKeysState[keyCode] &&
		m_lastKeysState[keyCode];
}

bool Input::getMouse(MouseButton button)
{
	if (button < 0) return false;
	return m_currentMouseButtonsState[button];
}

bool Input::getMouseDown(MouseButton button)
{
	if (button < 0) return false;
	return m_currentMouseButtonsState[button] &&
		!m_lastMouseButtonsState[button];
}

bool Input::getMouseUp(MouseButton button)
{
	if (button < 0) return false;
	return !m_currentMouseButtonsState[button] &&
		m_lastMouseButtonsState[button];
}

vec2 Input::getMousePosition()
{
	return m_mousePosition;
}

int Input::getMouseWheelDelta()
{
	return m_mouseWheelDelta;
}
