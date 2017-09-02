#pragma once

#include <fstream>
#include <vector>

#include "Stuff.h"

class Figure {
public:
	typedef unsigned char Position;
	typedef unsigned char Description;

	enum Color : unsigned char {
		White = 0x00,
		Black = 0x40
	};

	enum Type : unsigned char {
		Pawn,
		Rook,
		Knight,
		Bishop,
		Queen,
		King,
		Empty = 0x3F
	};

	Figure();
	Figure(Description description);
	Figure(Description description, Position position);
	
	vec2c get_pos_vec() const;
	Position get_pos() const;
	Description get_description() const;
	Figure::Color get_color() const;
	Figure::Type get_type() const;
	int get_pos_x() const;
	int get_pos_y() const;

	void set_pos(Position new_pos);
	void set_description(Description description);

	bool has_moved() const;

	static int get_pos_x(Figure::Position pos);
	static int get_pos_y(Figure::Position pos);
	static int get_pos_x(const wchar_t& xy);
	static int get_pos_y(const wchar_t& xy);
	static vec2c get_pos_vec(Figure::Position pos);
	static Figure::Position get_pos(int x, int y);
	static Figure::Position get_pos(const wchar_t& xy);
	static bool type_is_valid(Figure::Type type);
private:
	Description m_description;
	Position m_position;

	bool m_has_moved;
};