#include "Figure.h"

Figure::Figure(Description description) : m_description(description), m_position(0xFF)
{
}

Figure::Figure(Description description, Position position) : m_description(description), m_position(position)
{
}

Figure::Position Figure::get_position() const
{
	return m_position;
}

Figure::Description Figure::get_description() const
{
	return m_description;
}

unsigned char Figure::get_color() const
{
	return (m_description & 0xC0);
}

unsigned char Figure::get_type() const
{
	return (m_description & 0x38);
}

unsigned char Figure::get_id() const
{
	return (m_description & 0x07);
}

void Figure::move(Position new_pos)
{
	m_position = new_pos;
}

int Figure::get_pos_x()
{
	return static_cast<int>(m_position & 0xF0);
}

int Figure::get_pos_y()
{
	return static_cast<int>(m_position & 0x0F);
}