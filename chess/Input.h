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
	static bool getKey(Key keyCode);

	// Произошло ли нажатие клавиши в этом кадре
	static bool getKeyDown(Key keyCode);

	// Отпустили ли клавишу в этом кадре
	static bool getKeyUp(Key keyCode);


	// Нажата ли кнопка мыши в текущий момент
	static bool getMouse(MouseButton button);

	// Произошло ли нажатие кнопки мыши в этом кадре
	static bool getMouseDown(MouseButton button);

	// Отпустили ли кнопку мыши в этом кадре
	static bool getMouseUp(MouseButton button);

	// Позиция курсора относительно левого верхнего угла окна
	static vec2 getMousePosition();

	// Количество тиков колёсика мыши
	//@ если положительно, то колесо крутиться вверх
	//@ если отрицательное, то вниз
	static int getMouseWheelDelta();

private:
	friend class Core;

	static const int NUM_KEYS;
	static const int NUM_MOUSEBUTTONS;

	static vec2 m_mousePosition;
	static int m_mouseWheelDelta;

	static std::vector<bool> m_currentKeysState;
	static std::vector<bool> m_lastKeysState;

	static std::vector<bool> m_currentMouseButtonsState;
	static std::vector<bool> m_lastMouseButtonsState;
};