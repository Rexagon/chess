#pragma once

#include <functional>
#include <vector>
#include <map>

#include "Figure.h"

class Room;

class ChessBoard {
public:
	ChessBoard(Room* room);
	~ChessBoard();
	
	void init();
private:
	Room* m_room;

	std::vector<Figure> m_figures;

	static std::map<Figure::Type, std::function<void(Figure::Position, Figure*, std::vector<Figure>*)>> m_figure_handlers;
};