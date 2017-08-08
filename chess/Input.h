#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "Math.h"

using Key = sf::Keyboard::Key;
using MouseButton = sf::Mouse::Button;

class Input
{
public:
	// Обновляет состояния клавиш
	static void update();

	// Нажата ли клавиша в текущий момент
	static bool get_key(Key keyCode);

	// Произошло ли нажатие клавиши в этом кадре
	static bool get_key_down(Key keyCode);

	// Отпустили ли клавишу в этом кадре
	static bool get_key_up(Key keyCode);


	// Нажата ли кнопка мыши в текущий момент
	static bool get_mouse(MouseButton button);

	// Произошло ли нажатие кнопки мыши в этом кадре
	static bool get_mouse_down(MouseButton button);

	// Отпустили ли кнопку мыши в этом кадре
	static bool get_mouse_up(MouseButton button);

	// Позиция курсора относительно левого верхнего угла окна
	static vec2 get_mouse_position();

	// Количество тиков колёсика мыши
	//@ если положительно, то колесо крутиться вверх
	//@ если отрицательное, то вниз
	static int get_mouse_wheel_delta();

private:
	friend class Core;

	static const int NUM_KEYS;
	static const int NUM_MOUSEBUTTONS;

	static vec2 m_mouse_position;
	static int m_mouse_wheel_delta;

	static std::vector<bool> m_current_keys_state;
	static std::vector<bool> m_last_keys_state;

	static std::vector<bool> m_current_mouse_buttons_state;
	static std::vector<bool> m_last_mouse_buttons_state;
};