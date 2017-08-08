#include "Input.h"

#include "Core.h"

const int Input::NUM_KEYS = sf::Keyboard::KeyCount;
const int Input::NUM_MOUSEBUTTONS = sf::Mouse::ButtonCount;

vec2 Input::m_mouse_position = vec2(0, 0);
int Input::m_mouse_wheel_delta = 0;

std::vector<bool> Input::m_current_keys_state = std::vector<bool>(NUM_KEYS, false);
std::vector<bool> Input::m_last_keys_state = std::vector<bool>(NUM_KEYS, false);

std::vector<bool> Input::m_current_mouse_buttons_state = std::vector<bool>(NUM_MOUSEBUTTONS, false);
std::vector<bool> Input::m_last_mouse_buttons_state = std::vector<bool>(NUM_MOUSEBUTTONS, false);


void Input::update()
{
	m_last_keys_state = m_current_keys_state;
	m_last_mouse_buttons_state = m_current_mouse_buttons_state;
	m_mouse_wheel_delta = 0;
}

bool Input::get_key(Key keyCode)
{
	if (keyCode < 0) return false;
	return m_current_keys_state[keyCode];
}

bool Input::get_key_down(Key keyCode)
{
	if (keyCode < 0) return false;
	return m_current_keys_state[keyCode] &&
		!m_last_keys_state[keyCode];
}

bool Input::get_key_up(Key keyCode)
{
	if (keyCode < 0) return false;
	return !m_current_keys_state[keyCode] &&
		m_last_keys_state[keyCode];
}

bool Input::get_mouse(MouseButton button)
{
	if (button < 0) return false;
	return m_current_mouse_buttons_state[button];
}

bool Input::get_mouse_down(MouseButton button)
{
	if (button < 0) return false;
	return m_current_mouse_buttons_state[button] &&
		!m_last_mouse_buttons_state[button];
}

bool Input::get_mouse_up(MouseButton button)
{
	if (button < 0) return false;
	return !m_current_mouse_buttons_state[button] &&
		m_last_mouse_buttons_state[button];
}

vec2 Input::get_mouse_position()
{
	return m_mouse_position;
}

int Input::get_mouse_wheel_delta()
{
	return m_mouse_wheel_delta;
}
