#pragma once

#include "Math.h"

class Figure {
public:
	enum Color {
		White,
		Black
	};

	enum Type {
		Pawn,
		Rook,
		Knight,
		Bishop,
		Queen,
		King,

		Empty = 0x3F
	};

	Figure(char description);
	Figure(char color, char type);

	void set_description(char description);
	char get_description() const;

	void set_position(char x, char y);
	void set_position(const vec2c& position);
	void set_position(char packed_position);
	vec2c get_position() const;
	char get_packed_position() const;

	void set_color(char color);
	char get_color() const;

	void set_type(char type);
	char get_type() const;

	void set_moved(bool moved = true);
	bool was_moved() const;

	static char pack_position(const vec2c& position);
	static vec2c unpack_position(char position);

private:
	vec2c m_position;

	char m_color;
	char m_type;

	bool m_was_moved;
};