#pragma once

#include <functional>
#include <utility>
#include <fstream>
#include <memory>
#include <vector>
#include <map>

#include "Figure.h"

#include "Truelog.h"

typedef std::vector<std::vector<std::unique_ptr<Figure>>> Board;

class Room;

class ChessBoard {
public:
	explicit ChessBoard(Room* room);
	~ChessBoard();
	
	void init();
	void reinit();

	// Return true, if figure was moved, otherwise - return false
	bool move(const std::wstring& displacement);

	// Return true, if figures were moved, otherwise - return false
	bool castling(const std::wstring& king_displacement, const std::wstring& rook_displacement);

	// Return true, if figure was changed, otherwise - return false
	bool pawn_changes(const std::wstring& pawn_position, const std::wstring& changes);

	Figure* get_figure(const vec2uc& pos) const;
	Figure* get_figure(int x, int y) const;
	Figure* get_figure(Figure::Position pos) const;
	Figure* get_figure(const wchar_t& pos) const;

	unsigned int get_stroke_number() const;
private:
	Room* m_room;

	Board m_board;

	unsigned int m_stroke_number;

	std::wstring m_board_file_path;
	std::fstream m_board_file;

	void setup_default_board();

	std::map<Figure::Type, std::function<std::vector<vec2uc>(Figure*)>> m_possible_displacement;
};