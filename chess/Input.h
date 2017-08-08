#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "Math.h"

using Key = sf::Keyboard::Key;
using MouseButton = sf::Mouse::Button;

class Input
{
public:
	// ��������� ��������� ������
	static void update();

	// ������ �� ������� � ������� ������
	static bool get_key(Key keyCode);

	// ��������� �� ������� ������� � ���� �����
	static bool get_key_down(Key keyCode);

	// ��������� �� ������� � ���� �����
	static bool get_key_up(Key keyCode);


	// ������ �� ������ ���� � ������� ������
	static bool get_mouse(MouseButton button);

	// ��������� �� ������� ������ ���� � ���� �����
	static bool get_mouse_down(MouseButton button);

	// ��������� �� ������ ���� � ���� �����
	static bool get_mouse_up(MouseButton button);

	// ������� ������� ������������ ������ �������� ���� ����
	static vec2 get_mouse_position();

	// ���������� ����� ������� ����
	//@ ���� ������������, �� ������ ��������� �����
	//@ ���� �������������, �� ����
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