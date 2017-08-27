#include "ChessBoard.h"

#include "Room.h"

ChessBoard::ChessBoard(Room * room) : m_room(room), m_board(8), m_stroke_number(0)
{
	m_board_file_path = L"./data/rooms/" + m_room->get_name() + L".log";

	for (Board::iterator it_row = m_board.begin(); it_row != m_board.end(); it_row++) {
		(*it_row).resize(8);
	}

	m_possible_displacement = {
		{
			Figure::Type::Pawn,
			[this](Figure* figure) {
				//std::vector<vec2uc> movement;
				std::vector<vec2uc> possible_moves;
				vec2uc figure_pos = figure->get_pos_vec();
				vec2uc forward;
				vec2uc diagonal_left;
				vec2uc diagonal_right;

				if (figure->get_color() == Figure::Color::White) {
					diagonal_left = figure_pos + vec2uc(1, 1);
					diagonal_right = figure_pos + vec2uc(-1, 1);

					forward = figure_pos + vec2uc(0, 1);
					if (forward.y < 7 && get_figure(forward) == nullptr) {
						possible_moves.push_back(forward);

						forward = figure_pos + vec2uc(0, 2);
						if (figure_pos.y == 1 && get_figure(forward) == nullptr) {
							possible_moves.push_back(forward);
						}
					}

					if (get_figure(diagonal_left) != nullptr) {
						if (get_figure(diagonal_left)->get_color() != figure->get_color()) {
							possible_moves.push_back(diagonal_left);
						}
					}
					if (get_figure(diagonal_right) != nullptr) {
						if (get_figure(diagonal_right)->get_color() != figure->get_color()) {
							possible_moves.push_back(diagonal_right);
						}
					}
						
				}
				else {
					forward = figure_pos + vec2uc(0, -1);
					if (m_board[forward.x][forward.y].get() == nullptr && forward.y > 0) {
						possible_moves.push_back(vec2uc(0, -1));
					}

					forward = figure_pos + vec2uc(0, -2);
					if (m_board[forward.x][forward.y].get() == nullptr && figure_pos.y == 6) {
						possible_moves.push_back(vec2uc(0, -2));
					}
				}

				return possible_moves;
			}
		},

		{
			Figure::Type::Rook,
			[this](Figure* figure) {
				std::vector<vec2uc> possible_moves;
				std::vector<vec2uc> movement = {
					vec2uc(0, 1),
					vec2uc(1, 0),
					vec2uc(0, -1),
					vec2uc(-1, 0)
				};
				vec2uc figure_pos = figure->get_pos_vec();
				vec2uc current_move_pos = figure_pos;

				for (std::vector<vec2uc>::iterator it_move = movement.begin(); it_move != movement.end(); it_move++) {
					while (true) {
						current_move_pos += (*it_move);
						if (current_move_pos.y <= 7 && current_move_pos.y >= 0 &&
							current_move_pos.x <= 7 && current_move_pos.x >= 0 &&
							get_figure(current_move_pos) == nullptr)
						{
							possible_moves.push_back(current_move_pos);
						}
						else {
							if (get_figure(current_move_pos) != nullptr) {
								if (get_figure(current_move_pos)->get_color() != figure->get_color()) {
									possible_moves.push_back(current_move_pos);
									break;
								}
							}
							else {
								break;
							}
						}
					}
				}
				
				return possible_moves;
			}
		},

		{
			Figure::Type::Knight,
			[this](Figure* figure) {
				std::vector<vec2uc> possible_moves;
				std::vector<vec2uc> movement = {
					vec2uc(-1, 2),
					vec2uc(1, 2),
					vec2uc(2, 1),
					vec2uc(2, -1),
					vec2uc(1, -2),
					vec2uc(-1, -2),
					vec2uc(-2, -1),
					vec2uc(-2, 1)
				};
				vec2uc figure_pos = figure->get_pos_vec();
				vec2uc current_move_pos = figure_pos;

				for (std::vector<vec2uc>::iterator it_move = movement.begin(); it_move != movement.end(); it_move++) {
					current_move_pos += (*it_move);
					if (current_move_pos.y <= 7 && current_move_pos.y >= 0 &&
						current_move_pos.x <= 7 && current_move_pos.x >= 0 &&
						get_figure(current_move_pos) == nullptr)
					{
						possible_moves.push_back(current_move_pos);
					}
					else {
						if (get_figure(current_move_pos) != nullptr) {
							if (get_figure(current_move_pos)->get_color() != figure->get_color()) {
								possible_moves.push_back(current_move_pos);
							}
						}
					}
				}

				return possible_moves;
			}
		},

		{
			Figure::Type::Bishop,
			[this](Figure* figure) {
				std::vector<vec2uc> possible_moves;
				std::vector<vec2uc> movement = {
					vec2uc(1, 1),
					vec2uc(1, -1),
					vec2uc(-1, -1),
					vec2uc(-1, 1)
				};
				vec2uc figure_pos = figure->get_pos_vec();
				vec2uc current_move_pos = figure_pos;

				for (std::vector<vec2uc>::iterator it_move = movement.begin(); it_move != movement.end(); it_move++) {
					while (true) {
						current_move_pos += (*it_move);
						if (current_move_pos.y <= 7 && current_move_pos.y >= 0 &&
							current_move_pos.x <= 7 && current_move_pos.x >= 0 &&
							get_figure(current_move_pos) == nullptr)
						{
							possible_moves.push_back(current_move_pos);
						}
						else {
							if (get_figure(current_move_pos) != nullptr) {
								if (get_figure(current_move_pos)->get_color() != figure->get_color()) {
									possible_moves.push_back(current_move_pos);
									break;
								}
							}
							else {
								break;
							}
						}
					}
				}

				return possible_moves;
			}
		},

		{
			Figure::Type::Queen,
			[this](Figure* figure) {
				std::vector<vec2uc> possible_moves;
				std::vector<vec2uc> movement = {
					vec2uc(0, 1),
					vec2uc(1, 0),
					vec2uc(0, -1),
					vec2uc(-1, 0),
					vec2uc(1, 1),
					vec2uc(1, -1),
					vec2uc(-1, -1),
					vec2uc(-1, 1)
				};
				vec2uc figure_pos = figure->get_pos_vec();
				vec2uc current_move_pos = figure_pos;

				for (std::vector<vec2uc>::iterator it_move = movement.begin(); it_move != movement.end(); it_move++) {
					while (true) {
						current_move_pos += (*it_move);
						if (current_move_pos.y <= 7 && current_move_pos.y >= 0 &&
							current_move_pos.x <= 7 && current_move_pos.x >= 0 &&
							get_figure(current_move_pos) == nullptr)
						{
							possible_moves.push_back(current_move_pos);
						}
						else {
							if (get_figure(current_move_pos) != nullptr) {
								if (get_figure(current_move_pos)->get_color() != figure->get_color()) {
									possible_moves.push_back(current_move_pos);
									break;
								}
							}
							else {
								break;
							}
						}
					}
				}

				return possible_moves;
			}
		},

		{
			Figure::Type::King,
			[this](Figure* figure) {
				std::vector<vec2uc> possible_moves;
				std::vector<vec2uc> movement = {
					vec2uc(0, 1),
					vec2uc(1, 0),
					vec2uc(0, -1),
					vec2uc(-1, 0),
					vec2uc(1, 1),
					vec2uc(1, -1),
					vec2uc(-1, -1),
					vec2uc(-1, 1)
				};
				vec2uc figure_pos = figure->get_pos_vec();
				vec2uc current_move_pos = figure_pos;

				for (std::vector<vec2uc>::iterator it_move = movement.begin(); it_move != movement.end(); it_move++) {
					current_move_pos += (*it_move);
					if (current_move_pos.y <= 7 && current_move_pos.y >= 0 &&
						current_move_pos.x <= 7 && current_move_pos.x >= 0 &&
						get_figure(current_move_pos) == nullptr)
					{
						possible_moves.push_back(current_move_pos);
					}
					else {
						if (get_figure(current_move_pos) != nullptr) {
							if (get_figure(current_move_pos)->get_color() != figure->get_color()) {
								possible_moves.push_back(current_move_pos);
							}
						}
					}
				}

				return possible_moves;
			}
		}
	};
}

ChessBoard::~ChessBoard()
{
	m_board_file.close();
}

void ChessBoard::init()
{
	m_board_file.open(m_board_file_path, std::ios_base::out | std::ios_base::binary | std::ios_base::app);

	if (!m_board_file.is_open()) {
		throw std::exception("Board log could not open");
	}

	setup_default_board();

	Truelog::sync_print([&]() {
		Truelog::stream() << Truelog::Type::Info << "Chess board was initialize (room=" << m_room->get_name() << ")";
	});
}

void ChessBoard::reinit()
{
	if (m_board_file.is_open()) {
		m_board_file.close();
	}

	m_board_file.open(m_board_file_path, std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
	setup_default_board();

	Truelog::sync_print([&]() {
		Truelog::stream() << Truelog::Type::Info << "Chess board was restart (room=" << m_room->get_name() << ")";
	});
}

bool ChessBoard::move(const std::wstring& displacement)
{
	if (displacement.size() != 2) {
		return false;
	}

	Figure::Position current_pos = static_cast<Figure::Position>(displacement.at(0));
	Figure::Position new_pos = static_cast<Figure::Position>(displacement.at(1));
	Figure* current_figure = get_figure(current_pos);

	// Checking for the existence of the figure
	if (current_figure == nullptr) {
		return false;
	}

	bool can_move = false;
	std::vector<vec2uc> possible_displacement = m_possible_displacement[current_figure->get_type()](current_figure);
	for (std::vector<vec2uc>::iterator it_displacement = possible_displacement.begin(); it_displacement != possible_displacement.end(); it_displacement++) {
		if (Figure::get_pos_vec(new_pos) == (*it_displacement)) {
			can_move = true;
			break;
		}
	}

	if (can_move) {
		m_board[Figure::get_pos_x(new_pos)][Figure::get_pos_y(new_pos)].reset(m_board[Figure::get_pos_x(current_pos)][Figure::get_pos_y(current_pos)].release());
		m_stroke_number++;
		return true;
	}
	else {
		return false;
	}
}

bool ChessBoard::castling(const std::wstring & king_displacement, const std::wstring & rook_displacement)
{
	if (king_displacement.size() != 2 || rook_displacement.size() != 2) {
		return false;
	}

	Figure::Position new_king_pos = static_cast<Figure::Position>(king_displacement.at(1));
	Figure::Position new_rook_pos = static_cast<Figure::Position>(rook_displacement.at(1));

	Figure* king = get_figure(king_displacement.at(0));
	Figure* rook = get_figure(rook_displacement.at(0));

	if (king != nullptr && rook != nullptr) {
		if (king->get_type() == Figure::Type::King && !king->has_moved() &&
			rook->get_type() == Figure::Type::Rook && !rook->has_moved())
		{
			int begin_x;
			int end_x;
			if (king->get_pos_x() > rook->get_pos_x()) {
				begin_x = rook->get_pos_x();
				end_x = king->get_pos_x();
			}
			else {
				begin_x = king->get_pos_x();
				end_x = rook->get_pos_x();
			}

			// Checking possibility Castling
			for (int i = begin_x + 1; i < end_x; i++) {
				vec2uc pos(i, king->get_pos_y());
				if (get_figure(pos) != nullptr) {
					return false;
				}
				for (Board::iterator it_column = m_board.begin(); it_column != m_board.end(); it_column++) {
					for (std::vector<std::unique_ptr<Figure>>::iterator it_figure = it_column->begin(); it_figure != it_column->end(); it_figure++) {
						if (it_figure->get() != nullptr) {
							if ((*it_figure)->get_color() != king->get_color()) {
								std::vector<vec2uc> possible_check = m_possible_displacement[(*it_figure)->get_type()](it_figure->get());
								for (std::vector<vec2uc>::iterator it_possible_check = possible_check.begin(); it_possible_check != possible_check.end(); it_possible_check++) {
									if ((*it_possible_check) == pos) {
										return false;
									}
								}
							}
						}
					}
				}
			}

			// Castling...
			// Long castling
			if (king->get_pos_x() > rook->get_pos_x()) {
				m_board[2][king->get_pos_y()].reset(m_board[king->get_pos_x()][king->get_pos_y()].release());
				m_board[3][rook->get_pos_y()].reset(m_board[rook->get_pos_x()][rook->get_pos_y()].release());
			}
			// Short castling
			else {
				m_board[6][king->get_pos_y()].reset(m_board[king->get_pos_x()][king->get_pos_y()].release());
				m_board[5][rook->get_pos_y()].reset(m_board[rook->get_pos_x()][rook->get_pos_y()].release());
			}

			m_stroke_number++;
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

bool ChessBoard::pawn_changes(const std::wstring & pawn_position, const std::wstring & changes)
{
	if (changes.size() != 1) {
		return false;
	}

	Figure::Position pawn_current_pos = static_cast<Figure::Position>(pawn_position[1]);
	Figure::Type pawn_changing_type = static_cast<Figure::Type>(changes[0]);
	Figure::Color pawn_color = get_figure(pawn_current_pos)->get_color();

	if (!Figure::type_is_valid(pawn_changing_type)) {
		return false;
	}

	m_board[Figure::get_pos_x(pawn_current_pos)][Figure::get_pos_y(pawn_current_pos)].reset(new Figure(pawn_changing_type | pawn_color, pawn_current_pos));

	return true;
}

void ChessBoard::setup_default_board()
{
	Truelog::sync_print([&]() {
		Truelog::stream() << Truelog::Type::Info << "Setting default position of board";
	});

	// Reset whole board to nullptr
	for (Board::iterator it_row = m_board.begin(); it_row != m_board.end(); it_row++) {
		for (std::vector<std::unique_ptr<Figure>>::iterator it_figure = (*it_row).begin(); it_figure != (*it_row).end(); it_figure++) {
			(*it_figure).reset(nullptr);
		}
	}

	// Fill board with new figures
	// Pawns
	for (int i = 0; i < 8; i++) {
		m_board[i][1] = std::make_unique<Figure>(Figure(Figure::Color::White | Figure::Type::Pawn, Figure::get_pos(i, 1)));
		m_board[i][6] = std::make_unique<Figure>(Figure(Figure::Color::Black | Figure::Type::Pawn, Figure::get_pos(i, 6)));
	}

	// Rooks
	{
		m_board[0][0] = std::make_unique<Figure>(Figure(Figure::Color::White | Figure::Type::Rook, Figure::get_pos(0, 0)));
		m_board[7][0] = std::make_unique<Figure>(Figure(Figure::Color::White | Figure::Type::Rook, Figure::get_pos(7, 0)));

		m_board[0][7] = std::make_unique<Figure>(Figure(Figure::Color::Black | Figure::Type::Rook, Figure::get_pos(0, 7)));
		m_board[7][7] = std::make_unique<Figure>(Figure(Figure::Color::Black | Figure::Type::Rook, Figure::get_pos(7, 7)));
	}

	// Knights
	{
		m_board[1][0] = std::make_unique<Figure>(Figure(Figure::Color::White | Figure::Type::Knight, Figure::get_pos(1, 0)));
		m_board[6][0] = std::make_unique<Figure>(Figure(Figure::Color::White | Figure::Type::Knight, Figure::get_pos(6, 0)));

		m_board[1][7] = std::make_unique<Figure>(Figure(Figure::Color::Black | Figure::Type::Knight, Figure::get_pos(1, 7)));
		m_board[6][7] = std::make_unique<Figure>(Figure(Figure::Color::Black | Figure::Type::Knight, Figure::get_pos(6, 7)));
	}

	// Bishops
	{
		m_board[2][0] = std::make_unique<Figure>(Figure(Figure::Color::White | Figure::Type::Bishop, Figure::get_pos(2, 0)));
		m_board[5][0] = std::make_unique<Figure>(Figure(Figure::Color::White | Figure::Type::Bishop, Figure::get_pos(5, 0)));

		m_board[2][7] = std::make_unique<Figure>(Figure(Figure::Color::Black | Figure::Type::Bishop, Figure::get_pos(2, 7)));
		m_board[5][7] = std::make_unique<Figure>(Figure(Figure::Color::Black | Figure::Type::Bishop, Figure::get_pos(5, 7)));
	}

	// Queens
	{
		m_board[3][0] = std::make_unique<Figure>(Figure(Figure::Color::White | Figure::Type::Queen, Figure::get_pos(3, 0)));

		m_board[3][7] = std::make_unique<Figure>(Figure(Figure::Color::Black | Figure::Type::Queen, Figure::get_pos(3, 7)));
	}

	// Kings
	{
		m_board[4][0] = std::make_unique<Figure>(Figure(Figure::Color::White | Figure::Type::King, Figure::get_pos(4, 0)));

		m_board[4][7] = std::make_unique<Figure>(Figure(Figure::Color::Black | Figure::Type::King, Figure::get_pos(4, 7)));
	}

	Truelog::sync_print([&]() {
		std::string filling_of_board = "";
		for (Board::iterator it_column = m_board.begin(); it_column != m_board.end(); it_column++) {
			for (std::vector<std::unique_ptr<Figure>>::iterator it_figure = it_column->begin(); it_figure != it_column->end(); it_figure++) {
				if ((*it_figure).get() == nullptr) {
					filling_of_board += " 00";
				}
				else {
					if ((*it_figure)->get_color() == Figure::Color::White) {
						filling_of_board += " W";
					}
					else {
						filling_of_board += " B";
					}
					switch ((*it_figure)->get_type()) {
					case Figure::Type::Pawn:
						filling_of_board += "p";
						break;
					case Figure::Type::Rook:
						filling_of_board += "r";
						break;
					case Figure::Type::Knight:
						filling_of_board += "n";
						break;
					case Figure::Type::Bishop:
						filling_of_board += "b";
						break;
					case Figure::Type::Queen:
						filling_of_board += "q";
						break;
					case Figure::Type::King:
						filling_of_board += "k";
						break;
					}
				}
			}
			filling_of_board += "\n";
		}
		Truelog::stream() << Truelog::Type::Debug << "Filling of board after default setup (room=" << m_room->get_name() <<"): \n" << filling_of_board;
	});
}

Figure * ChessBoard::get_figure(const vec2uc& pos) const
{
	if (pos.x > 7 || pos.x < 0 ||
		pos.y > 7 || pos.y < 0)
	{
		return nullptr;
	}

	return m_board[pos.x][pos.y].get();
}

Figure * ChessBoard::get_figure(int x, int y) const
{
	return get_figure(vec2uc(x, y));
}

Figure * ChessBoard::get_figure(Figure::Position pos) const
{
	return get_figure(vec2uc(Figure::get_pos_x(pos), Figure::get_pos_y(pos)));
}

Figure * ChessBoard::get_figure(const wchar_t& pos) const
{
	return get_figure(vec2uc(Figure::get_pos_x(pos), Figure::get_pos_y(pos)));
}

unsigned int ChessBoard::get_stroke_number() const
{
	return m_stroke_number;
}
