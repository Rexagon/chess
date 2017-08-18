#pragma once

#include <memory>
#include <stack>

#include "AssetManager.h"
#include "SoundManager.h"
#include "ConfigManager.h"
#include "CursorManager.h"
#include "Client.h"
#include "State.h"
#include "Input.h"
#include "GUI.h"
#include "Log.h"

class Core
{
public:
	// Подготавливает ядро к запуску
	static void init();

	// Очищает ресурсы ядра
	static void close();

	// Запускает основной цикл
	static void run();

	// Останавливает основной цикл
	static void stop();

	// Рисует объект
	static void draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default);

	// Рисует объект заданным шейдером
	static void draw(const sf::Drawable& drawable, sf::Shader* shader);

	// Рисует вершины
	static void draw(const sf::Vertex* vertices, std::size_t vertex_count,
		sf::PrimitiveType type, const sf::RenderStates& states = sf::RenderStates::Default);

	// Возвращает объект окна игры
	static sf::RenderWindow* get_window() { return &m_window; }

	// Добавляет новое состояние
	template<class T, class... Args>
	static void add_state(Args&&... args)
	{
		static_assert(std::is_base_of<State, T>::value, "Core::AddState<T>() - T must be child of State");

		if (!m_states.empty()) {
			State* state = m_states.top().get();
			state->scene_leave();
			state->m_gui.press_item(nullptr);
			state->m_gui.focus_item(nullptr);
			state->m_gui.hover_item(nullptr);
		}

		CursorManager::set_style(CursorManager::Normal);

		m_states.push(std::unique_ptr<T>(new T(std::forward<Args>(args)...)));
		m_states.top()->init();
	}

	// Подменяет текущее состояние другим
	template<class T, class... Args>
	static void change_state(Args&&... args)
	{
		delete_state();
		add_state<T>(std::forward(args)...);
	}

	// Удаляет текущее состояние
	static void delete_state()
	{
		if (!m_states.empty()) {
			m_states.top()->close();
			m_states.pop();
		}

		if (!m_states.empty()) {
			m_states.top()->scene_return();
		}
	}

	// Возвращает текущее состояние
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