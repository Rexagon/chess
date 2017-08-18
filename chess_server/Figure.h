#pragma once

class Figure {
public:
	typedef unsigned char Position;
	typedef unsigned char Description;

	enum Color : unsigned char {
		White = 0x00,	// 00......
		Black = 0xC0	// 11......
	};

	enum Type : unsigned char {
		Pawn = 0x00,	// ..000...
		Rook = 0x08,	// ..001...
		Knight = 0x10,	// ..010...
		Bishop = 0x18,	// ..011...
		Queen = 0x20,	// ..100...
		King = 0x28,	// ..101...
	};

	Figure(Description description);
	Figure(Description description, Position position);
	
	Position get_position() const;
	Description get_description() const;
	unsigned char get_color() const;
	unsigned char get_type() const;
	unsigned char get_id() const;
	int get_pos_x();
	int get_pos_y();

	void move(Position new_pos);

private:
	Description m_description;
	Position m_position;
};