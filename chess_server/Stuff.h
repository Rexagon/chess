#pragma once

#include <string>
#include <time.h>

#include <SFML/System.hpp>

class Figure;

typedef sf::Vector2<char> vec2c;

namespace std {
	std::wstring to_wstring(const std::string& str);
}

std::wostream & operator<<(std::wostream& stream, const std::string& s);

namespace Stuff {
	time_t get_current_time();

	std::string time_to_string(time_t time);
	std::wstring time_to_wstring(time_t time);
}
