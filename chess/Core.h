#pragma once

#include <memory>
#include <stack>

#include "AssetManager.h"
#include "SoundManager.h"
#include "Cursor.h"
#include "State.h"
#include "Input.h"
#include "GUI.h"
#include "Log.h"

class Core
{
public:
	// �������������� ���� � �������
	static void init();

	// ������� ������� ����
	static void close();

	// ��������� �������� ����
	static void run();

	// ������������� �������� ����
	static void stop();

	// ������ ������
	static void draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default);

	// ������ ������ �������� ��������
	static void draw(const sf::Drawable& drawable, sf::Shader* shader);

	// ������ �������
	static void draw(const sf::Vertex* vertices, std::size_t vertex_count,
		sf::PrimitiveType type, const sf::RenderStates& states = sf::RenderStates::Default);

	// ���������� ������ ���� ����
	static sf::RenderWindow* get_window() { return &m_window; }

	// ��������� ����� ���������
	template<class T, class... Args>
	static void add_state(Args&&... args)
	{
		static_assert(std::is_base_of<State, T>::value, "Core::AddState<T>() - T must be child of State");

		if (!m_states.empty()) {
			m_states.top()->pause();
		}

		m_states.push(std::unique_ptr<T>(new T(std::forward<Args>(args)...)));
		m_states.top()->init();
	}

	// ��������� ������� ��������� ������
	template<class T, class... Args>
	static void change_state(Args&&... args)
	{
		delete_state();
		add_state<T>(std::forward(args)...);
	}

	// ������� ������� ���������
	static void delete_state()
	{
		if (!m_states.empty()) {
			m_states.top()->close();
			m_states.pop();
		}

		if (!m_states.empty()) {
			m_states.top()->resume();
		}
	}

	// ���������� ������� ���������
	static State* get_state()
	{
		if (!m_states.empty()) {
			return m_states.top().get();
		}
		else {
			return nullptr;
		}
	}
private:
	static void handle_input(const float dt);

	static bool m_is_running;
	static sf::RenderWindow m_window;
	static std::stack<std::unique_ptr<State>> m_states;
};