#include "ChessBoard.h"

#include "Room.h"

std::map<Figure::Type, std::function<void(Figure::Position, Figure*, std::vector<Figure>*)>> ChessBoard::m_figure_handlers = {
	{
		Figure::Type::Pawn,
		[](Figure::Position new_pos, Figure* figure, std::vector<Figure>* figures) {
			if (figure->get_color() == Figure::Color::White) {
				if (figure->get_pos_y() == 1) {

				}
			}
			else {

			}
			
			figure->move(new_pos);
		}
	}
};

ChessBoard::ChessBoard(Room * room) : m_room(room)
{
	m_figures = {

	};
}

ChessBoard::~ChessBoard()
{
}

void ChessBoard::init()
{
}