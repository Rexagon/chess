#pragma once

#include <vector>
#include <cmath>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/Rect.hpp>

using vec2 = sf::Vector2f;
using vec2i = sf::Vector2i;
using vec2u = sf::Vector2u;
using vec2c = sf::Vector2<char>;

using vec3 = sf::Vector3f;
using vec3i = sf::Vector3i;

using rect = sf::FloatRect;
using recti = sf::IntRect;

namespace math
{
	// Константы

	const float PI = 3.141592653f;
	const float E = 2.718281828f;


	// Скалярные функции

	// Возвращает знак аргумента
	template <typename T>
	int sign(T val)
	{
		return (T(0) < val) - (val < T(0));
	}

	// Приводит value к диапазону [min, max]
	//@ если value > max, то вернёт max
	//@ если value < min, то вернёт min
	//@ иначе вернёт value
	float clamp(float value, float min, float max);

	// Переводит из градусов в радианы
	float to_radians(float degrees);

	// Переводит из радиан в градусы
	float to_degrees(float radians);

	// Получает значение функции в x, заданной точками
	//@ используется линейная интерполяция
	float interpolate(const std::vector<std::pair<float, float>>& curve, float x);


	// Векторные функции

	// Возвращает длину вектора
	float length(const vec2& v);

	// Скалярное произведение между двумя векторами
	float dot(const vec2& a, const vec2& b);

	// Возвращает нормализованный вектор
	vec2 normalize(const vec2& v);

	// Возвращает вектор v, повёрнутый на угол angle
	//@ angle - угол в радианах
	vec2 rotate(const vec2& v, float angle);

	// Возвращает длину проекции b на a
	float scalar_projection(const vec2& a, const vec2& b);

	// Возвращает вектор проекции b на a
	vec2 vector_projection(const vec2& a, const vec2& b);
}