#pragma once

#include <memory>
#include <stack>

#include "AssetManager.h"
#include "SoundManager.h"
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
	static void draw(const sf::Vertex* vertices, std::size_t vertexCount,
		sf::PrimitiveType type, const sf::RenderStates& states = sf::RenderStates::Default);

	// Возвращает объект окна игры
	static sf::RenderWindow* getWindow() { return &m_window; }

	// Добавляет новое состояние
	template<class T, class... Args>
	static void addState(Args&&... args)
	{
		static_assert(std::is_base_of<State, T>::value, "Core::AddState<T>() - T must be child of State");

		if (!m_states.empty()) {
			m_states.top()->pause();
		}

		m_states.push(std::unique_ptr<T>(new T(std::forward<Args>(args)...)));
		m_states.top()->init();
	}

	// Подменяет текущее состояние другим
	template<class T, class... Args>
	static void changeState(Args&&... args)
	{
		deleteState();
		addState<T>(std::forward(args)...);
	}

	// Удаляет текущее состояние
	static void deleteState()
	{
		if (!m_states.empty()) {
			m_states.top()->close();
			m_states.pop();
		}

		if (!m_states.empty()) {
			m_states.top()->resume();
		}
	}

	// Возвращает текущее состояние
	static State* getState()
	{
		if (!m_states.empty()) {
			return m_states.top().get();
		}
		else {
			return nullptr;
		}
	}
private:
	static void HandleInput(const float dt);

	static bool m_isRunning;
	static sf::RenderWindow m_window;
	static std::stack<std::unique_ptr<State>> m_states;
};