#include "Figure.h"

Figure::Figure() : m_description(Figure::Type::Empty), m_position(0xFF), m_has_moved(false)
{
}

Figure::Figure(Description description) : m_description(description), m_position(0xFF), m_has_moved(false)
{
}

Figure::Figure(Description description, Position position) : m_description(description), m_position(position), m_has_moved(false)
{
}

vec2uc Figure::get_pos_vec() const
{
	return vec2uc(this->get_pos_x(), this->get_pos_y());
}

Figure::Position Figure::get_pos() const
{
	return m_position;
}

Figure::Description Figure::get_description() const
{
	return m_description;
}

Figure::Color Figure::get_color() const
{
	return static_cast<Figure::Color>(m_description & 0xC0);
}

Figure::Type Figure::get_type() const
{
	return static_cast<Figure::Type>(m_description & 0x3F);
}

void Figure::set_pos(Position new_pos)
{
	m_position = new_pos;
	m_has_moved = true;
}

void Figure::set_description(Description description)
{
	m_description = description;
}

bool Figure::has_moved() const
{
	return m_has_moved;
}

int Figure::get_pos_x(Figure::Position pos)
{
	return static_cast<int>(pos & 0xF0);
}

int Figure::get_pos_y(Figure::Position pos)
{
	return static_cast<int>(pos & 0x0F);
}

int Figure::get_pos_x(const wchar_t & xy)
{
	return Figure::get_pos_x(Figure::get_pos(xy));
}

int Figure::get_pos_y(const wchar_t & xy)
{
	return Figure::get_pos_y(Figure::get_pos(xy));
}

vec2uc Figure::get_pos_vec(Figure::Position pos)
{
	return vec2uc(Figure::get_pos_x(pos), Figure::get_pos_y(pos));
}

Figure::Position Figure::get_pos(int x, int y)
{
	Figure::Position pos = (static_cast<unsigned char>(x) << 4);
	pos |= static_cast<unsigned char>(y);
	return pos;
}

Figure::Position Figure::get_pos(const wchar_t & xy)
{
	unsigned char x = static_cast<unsigned char>(xy >> 4);
	unsigned char y = static_cast<unsigned char>(xy & 0xF);

	return Figure::get_pos(x, y);
}

bool Figure::type_is_valid(Figure::Type type)
{
	if (type >= Figure::Type::Pawn && type <= Figure::Type::King) {
		return true;
	}
	else {
		return false;
	}
}

int Figure::get_pos_x() const
{
	return static_cast<int>(m_position & 0xF0);
}

int Figure::get_pos_y() const
{
	return static_cast<int>(m_position & 0x0F);
}