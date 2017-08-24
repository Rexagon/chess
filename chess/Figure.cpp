#include "Figure.h"

Figure::Figure(char description) :
	m_position(127, 127), m_was_moved(false)
{
	set_description(description);
}

Figure::Figure(char color, char type) :
	m_color(color), m_type(type), m_position(127, 127), m_was_moved(false)
{
}

void Figure::set_description(char description)
{
	m_color = ((description & 0x40) >> 6) == Color::White;
	m_type = description & 0x3F;
}

char Figure::get_description() const
{
	return (m_color << 6) | (m_type & 0x3F);
}

void Figure::set_position(char x, char y)
{
	m_position = vec2c(x, y);
}

void Figure::set_position(const vec2c & position)
{
	m_position = position;
}

void Figure::set_position(char packed_position)
{
	m_position = vec2c((packed_position & 0xF0) >> 4, packed_position & 0x0F);
}

vec2c Figure::get_position() const
{
	return m_position;
}

char Figure::get_packed_position() const
{
	return (m_position.x << 4) | (m_position.y & 0x0F);
}

void Figure::set_color(char color)
{
	m_color = color;
}

char Figure::get_color() const
{
	return m_color;
}

void Figure::set_type(char type)
{
	m_type = type;
}

char Figure::get_type() const
{
	return m_type;
}

bool Figure::was_moved() const
{
	return m_was_moved;
}
