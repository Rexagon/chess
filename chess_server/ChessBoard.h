#pragma once

#include <functional>
#include <utility>
#include <fstream>
#include <memory>
#include <vector>
#include <cstdio>
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
	bool move(Figure::Position old_pos, Figure::Position new_pos);

	// Return true, if figures were moved, otherwise - return false
	bool castling(const std::wstring& king_displacement, const std::wstring& rook_displacement);
	bool castling(Figure::Position king_old_pos, Figure::Position king_new_pos, Figure::Position rook_old_pos, Figure::Position rook_new_pos);

	// Return true, if figure was changed, otherwise - return false
	bool changing_pawn(const std::wstring& pawn_position, const std::wstring& changing);
	bool changing_pawn(Figure::Position current_pos, unsigned char changing);

	Figure* get_figure(const vec2c& pos) const;
	Figure* get_figure(int x, int y) const;
	Figure* get_figure(Figure::Position pos) const;
	Figure* get_figure(const wchar_t& pos) const;

	unsigned int get_turn_number() const;

	void open_log_file(const std::wstring& path, int mods);

	std::vector<std::wstring> get_board_state();
private:
	Room* m_room;

	Board m_board;

	unsigned int m_turn_number;

	std::fstream m_board_file;

	void setup_default_board();
	// size = ...
	// (0) - end of the game
	// (2) - figure moving
	// (3) - pawn changing
	// (4) - castling
	void read_from_file();
	void write_to_file(const std::vector<unsigned char>& args);

	std::map<unsigned char, std::function<std::vector<vec2c>(Figure*)>> m_possible_displacement;
};