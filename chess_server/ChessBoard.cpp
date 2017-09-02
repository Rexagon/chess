#include "ChessBoard.h"

#include "Room.h"

ChessBoard::ChessBoard(Room * room) : m_room(room), m_board(8), m_turn_number(0)
{
	for (Board::iterator it_row = m_board.begin(); it_row != m_board.end(); it_row++) {
		(*it_row).resize(8);
	}

	m_possible_displacement = {
		{
			Figure::Type::Pawn,
			[this](Figure* figure) {
				//std::vector<vec2uc> movement;
				std::vector<vec2c> possible_moves;
				vec2c figure_pos = figure->get_pos_vec();
				vec2c forward;
				vec2c diagonal_left;
				vec2c diagonal_right;

				if (figure->get_color() == Figure::Color::White) {
					diagonal_left = figure_pos + vec2c(-1, 1);
					diagonal_right = figure_pos + vec2c(1, 1);

					forward = figure_pos + vec2c(0, 1);
					if (forward.y <= 7 && get_figure(forward) == nullptr) {
						possible_moves.push_back(forward);

						forward = figure_pos + vec2c(0, 2);
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
					diagonal_left = figure_pos + vec2c(-1, -1);
					diagonal_right = figure_pos + vec2c(1, -1);

					forward = figure_pos + vec2c(0, -1);
					if (forward.y >= 0 && get_figure(forward) == nullptr) {
						possible_moves.push_back(forward);

						forward = figure_pos + vec2c(0, -2);
						if (figure_pos.y == 6 && get_figure(forward) == nullptr) {
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

				return possible_moves;
			}
		},

		{
			Figure::Type::Rook,
			[this](Figure* figure) {
				std::vector<vec2c> possible_moves;
				std::vector<vec2c> movement = {
					vec2c(0, 1),
					vec2c(1, 0),
					vec2c(0, -1),
					vec2c(-1, 0)
				};
				vec2c figure_pos = figure->get_pos_vec();
				vec2c current_move_pos = figure_pos;

				for (std::vector<vec2c>::iterator it_move = movement.begin(); it_move != movement.end(); it_move++) {
					current_move_pos = figure_pos;
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
				std::vector<vec2c> possible_moves;
				std::vector<vec2c> movement = {
					vec2c(-1, 2),
					vec2c(1, 2),
					vec2c(2, 1),
					vec2c(2, -1),
					vec2c(1, -2),
					vec2c(-1, -2),
					vec2c(-2, -1),
					vec2c(-2, 1)
				};
				vec2c figure_pos = figure->get_pos_vec();
				vec2c current_move_pos = figure_pos;

				for (std::vector<vec2c>::iterator it_move = movement.begin(); it_move != movement.end(); it_move++) {
					current_move_pos = figure_pos + (*it_move);
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
				std::vector<vec2c> possible_moves;
				std::vector<vec2c> movement = {
					vec2c(1, 1),
					vec2c(1, -1),
					vec2c(-1, -1),
					vec2c(-1, 1)
				};
				vec2c figure_pos = figure->get_pos_vec();
				vec2c current_move_pos = figure_pos;

				for (std::vector<vec2c>::iterator it_move = movement.begin(); it_move != movement.end(); it_move++) {
					current_move_pos = figure_pos;
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
				std::vector<vec2c> possible_moves;
				std::vector<vec2c> movement = {
					vec2c(0, 1),
					vec2c(1, 0),
					vec2c(0, -1),
					vec2c(-1, 0),
					vec2c(1, 1),
					vec2c(1, -1),
					vec2c(-1, -1),
					vec2c(-1, 1)
				};
				vec2c figure_pos = figure->get_pos_vec();
				vec2c current_move_pos = figure_pos;

				for (std::vector<vec2c>::iterator it_move = movement.begin(); it_move != movement.end(); it_move++) {
					current_move_pos = figure_pos;
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
				std::vector<vec2c> possible_moves;
				std::vector<vec2c> movement = {
					vec2c(0, 1),
					vec2c(1, 0),
					vec2c(0, -1),
					vec2c(-1, 0),
					vec2c(1, 1),
					vec2c(1, -1),
					vec2c(-1, -1),
					vec2c(-1, 1)
				};
				vec2c figure_pos = figure->get_pos_vec();
				vec2c current_move_pos = figure_pos;

				for (std::vector<vec2c>::iterator it_move = movement.begin(); it_move != movement.end(); it_move++) {
					current_move_pos = figure_pos + (*it_move);
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
	setup_default_board();
	read_from_file();

	Truelog::sync_print([&]() {
		Truelog::stream() << Truelog::Type::Info << "Chess board was initialize (room=" << m_room->get_name() << ")";
	});
}

void ChessBoard::reinit()
{
	open_log_file(m_room->get_file_path(), std::ios::out | std::ios::trunc | std::ios::binary);
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
	std::vector<vec2c> possible_displacement = m_possible_displacement[current_figure->get_type()](current_figure);
	for (std::vector<vec2c>::iterator it_displacement = possible_displacement.begin(); it_displacement != possible_displacement.end(); it_displacement++) {
		if (Figure::get_pos_vec(new_pos) == (*it_displacement)) {
			can_move = true;
			break;
		}
	}

	if (can_move) {
		m_board[Figure::get_pos_x(new_pos)][Figure::get_pos_y(new_pos)].reset(m_board[Figure::get_pos_x(current_pos)][Figure::get_pos_y(current_pos)].release());
		m_board[Figure::get_pos_x(new_pos)][Figure::get_pos_y(new_pos)]->set_pos(new_pos);
		m_turn_number++;
		write_to_file(std::vector<unsigned char>({ current_pos, new_pos }));
		Truelog::sync_print([&]() {
			Truelog::stream() << Truelog::Type::Info << "Figure was moved"
				<< "\n{"
				<< "\n    description = " << current_figure->get_description()
				<< "\n    current_pos = (" << Figure::get_pos_x(current_pos) << ", " << Figure::get_pos_y(current_pos) << ")"
				<< "\n    new_pos     = (" << Figure::get_pos_x(new_pos) << ", " << Figure::get_pos_y(new_pos) << ")"
				<< "\n}";

			std::string filling_of_board = "";

			for (size_t i = 0; i < m_board.size(); ++i) {
				for (size_t j = 0; j < m_board.size(); ++j) {
					Figure* current_figure = m_board[j][7 - i].get();
					if (current_figure == nullptr) {
						filling_of_board += " __";
					}
					else {
						if (current_figure->get_color() == Figure::Color::White) {
							filling_of_board += " W";
						}
						else {
							filling_of_board += " B";
						}
						switch (current_figure->get_type()) {
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

			Truelog::stream() << Truelog::Type::Debug << "Current board's state (room=" << m_room->get_name() << "): \n" << filling_of_board;
		});

		return true;
	}
	else {
		return false;
	}
}

bool ChessBoard::move(Figure::Position old_pos, Figure::Position new_pos)
{
	std::wstring out = L"";
	out += static_cast<wchar_t>(old_pos);
	out += static_cast<wchar_t>(new_pos);
	return move(out);
}

bool ChessBoard::castling(const std::wstring & king_displacement, const std::wstring & rook_displacement)
{
	if (king_displacement.size() != 2 || rook_displacement.size() != 2) {
		return false;
	}
	
	Figure::Position king_current_pos = static_cast<Figure::Position>(king_displacement.at(0));
	Figure::Position rook_current_pos = static_cast<Figure::Position>(rook_displacement.at(0));
	Figure::Position king_new_pos = static_cast<Figure::Position>(king_displacement.at(1));
	Figure::Position rook_new_pos = static_cast<Figure::Position>(rook_displacement.at(1));

	Figure* king = get_figure(king_current_pos);
	Figure* rook = get_figure(rook_current_pos);

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
				vec2c pos(i, king->get_pos_y());
				if (get_figure(pos) != nullptr) {
					return false;
				}
				for (Board::iterator it_column = m_board.begin(); it_column != m_board.end(); it_column++) {
					for (std::vector<std::unique_ptr<Figure>>::iterator it_figure = it_column->begin(); it_figure != it_column->end(); it_figure++) {
						if (it_figure->get() != nullptr) {
							if ((*it_figure)->get_color() != king->get_color()) {
								std::vector<vec2c> possible_check = m_possible_displacement[(*it_figure)->get_type()](it_figure->get());
								for (std::vector<vec2c>::iterator it_possible_check = possible_check.begin(); it_possible_check != possible_check.end(); it_possible_check++) {
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
			m_turn_number++;
			write_to_file(std::vector<unsigned char>({ king_current_pos, king_new_pos, rook_current_pos, rook_new_pos }));
			
			Truelog::sync_print([&]() {
				Truelog::stream() << Truelog::Type::Info << "Castled"
					<< "\n{"
					<< "\n    " << king_current_pos
					<< "\n    " << king_new_pos
					<< "\n    " << rook_current_pos
					<< "\n    " << rook_new_pos
					<< "\n}";
			});

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

bool ChessBoard::castling(Figure::Position king_old_pos, Figure::Position king_new_pos, Figure::Position rook_old_pos, Figure::Position rook_new_pos)
{
	std::wstring king_out = L"";
	std::wstring rook_out = L"";

	king_out += static_cast<wchar_t>(king_old_pos);
	king_out += static_cast<wchar_t>(king_new_pos);

	rook_out += static_cast<wchar_t>(rook_old_pos);
	rook_out += static_cast<wchar_t>(rook_new_pos);

	return castling(king_out, rook_out);
}

bool ChessBoard::changing_pawn(const std::wstring & pawn_position, const std::wstring & changing)
{
	if (changing.size() != 1) {
		return false;
	}

	Figure::Position pawn_current_pos = static_cast<Figure::Position>(pawn_position[1]);
	Figure::Type pawn_changing_type = static_cast<Figure::Type>(changing[0]);
	Figure::Color pawn_color = get_figure(pawn_current_pos)->get_color();

	if (!Figure::type_is_valid(pawn_changing_type)) {
		return false;
	}

	m_board[Figure::get_pos_x(pawn_current_pos)][Figure::get_pos_y(pawn_current_pos)].reset(new Figure(pawn_changing_type | pawn_color, pawn_current_pos));
	write_to_file(std::vector<unsigned char>({ pawn_current_pos, pawn_changing_type, unsigned char() }));
	return true;
}

bool ChessBoard::changing_pawn(Figure::Position current_pos, unsigned char changing)
{
	std::wstring out_pos = L"";
	std::wstring out_type = L"";

	out_pos += wchar_t();
	out_pos += static_cast<wchar_t>(current_pos);

	out_type += (wchar_t)changing;

	return changing_pawn(out_pos, out_type);
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

		for (size_t i = 0; i < m_board.size(); ++i) {
			for (size_t j = 0; j < m_board.size(); ++j) {
				Figure* current_figure = m_board[j][7 - i].get();
				if (current_figure == nullptr) {
					filling_of_board += " __";
				}
				else {
					if (current_figure->get_color() == Figure::Color::White) {
						filling_of_board += " W";
					}
					else {
						filling_of_board += " B";
					}
					switch (current_figure->get_type()) {
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

void ChessBoard::read_from_file()
{
	Truelog::stream() << m_room->get_file_path();

	open_log_file(m_room->get_file_path(), std::ios::in | std::ios::binary);

	//m_board_file.seekg(0, m_board_file.beg);

	while (!m_board_file.eof()) {
		unsigned int size = 0;
		m_board_file.read(reinterpret_cast<char *>(&size), sizeof(size));

		switch (size) {
		case 2:
		{
			Figure::Position old_pos;
			Figure::Position new_pos;

			m_board_file.read(reinterpret_cast<char *>(&old_pos), sizeof(Figure::Position));
			m_board_file.read(reinterpret_cast<char *>(&new_pos), sizeof(Figure::Position));

			bool has_moved = move(old_pos, new_pos);
			if (!has_moved) {
				throw std::exception("Error during reading from file (figure movement)");
			}
			break;
		}
		case 3:
		{
			Figure::Position current_pos;
			unsigned char type;

			m_board_file.read(reinterpret_cast<char *>(&current_pos), sizeof(Figure::Position));
			m_board_file.read(reinterpret_cast<char *>(&type), sizeof(unsigned char));

			m_board_file.seekg(1, m_board_file.cur);

			bool has_changed = changing_pawn(current_pos, type);
			if (!has_changed) {
				throw std::exception("Error during reading from file (pawn changing)");
			}
			break;
		}
		case 4:
		{
			Figure::Position king_old_pos;
			Figure::Position king_new_pos;
			Figure::Position rook_old_pos;
			Figure::Position rook_new_pos;

			m_board_file.read(reinterpret_cast<char *>(&king_old_pos), sizeof(Figure::Position));
			m_board_file.read(reinterpret_cast<char *>(&king_new_pos), sizeof(Figure::Position));
			m_board_file.read(reinterpret_cast<char *>(&rook_old_pos), sizeof(Figure::Position));
			m_board_file.read(reinterpret_cast<char *>(&rook_new_pos), sizeof(Figure::Position));

			bool has_castled = castling(king_old_pos, king_new_pos, rook_old_pos, rook_new_pos);
			if (!has_castled) {
				throw std::exception("Error during reading from file (castling)");
			}
			break;
		}
		case 0:
		{
			Truelog::stream() << Truelog::Type::Error << "Size is zero";
			// TODO: make end of the game
			return;
		}
		default:
		{
			// TODO:: make error
			Truelog::stream() << Truelog::Type::Warning << "File is empty";
			return;
		}
		}
		m_board_file.seekg(4, m_board_file.cur);
	}

	m_board_file.close();
	open_log_file(m_room->get_file_path(), std::ios::out | std::ios::binary | std::ios::app);
}

void ChessBoard::write_to_file(const std::vector<unsigned char>& args)
{
	if (!m_board_file.is_open()) {
		Truelog::sync_print([&]() {
			Truelog::stream() << Truelog::Type::Warning << "Writing attend to log file failed (room=" << m_room->get_name() << ")";
		});
		return;
	}

	unsigned int size = static_cast<unsigned int>(args.size());
	m_board_file.write(reinterpret_cast<const char*>(&size), sizeof(unsigned int));
	for (size_t i = 0; i < args.size(); ++i) {
		m_board_file.write(reinterpret_cast<const char*>(&args[i]), sizeof(unsigned char));
	}
	m_board_file.write(reinterpret_cast<const char*>(&size), sizeof(unsigned int));

	m_board_file.flush();
}

Figure * ChessBoard::get_figure(const vec2c& pos) const
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
	return get_figure(vec2c(x, y));
}

Figure * ChessBoard::get_figure(Figure::Position pos) const
{
	return get_figure(vec2c(Figure::get_pos_x(pos), Figure::get_pos_y(pos)));
}

Figure * ChessBoard::get_figure(const wchar_t& pos) const
{
	return get_figure(vec2c(Figure::get_pos_x(pos), Figure::get_pos_y(pos)));
}

unsigned int ChessBoard::get_turn_number() const
{
	return m_turn_number;
}

void ChessBoard::open_log_file(const std::wstring & path, int mods)
{
	if (m_board_file.is_open()) {
		m_board_file.close();
	}

	m_board_file.open(path, mods);

	if (!m_board_file.is_open()) {
		throw std::exception("Board log could not open");
	}
}

std::vector<std::wstring> ChessBoard::get_board_state()
{
	std::vector<std::wstring> board_state;

	board_state.push_back(std::to_wstring(m_turn_number));
	
	for (Board::iterator it_column = m_board.begin(); it_column != m_board.end(); it_column++) {
		for (std::vector<std::unique_ptr<Figure>>::iterator it_figure = (*it_column).begin(); it_figure != (*it_column).end(); it_figure++) {
			if ((*it_figure) != nullptr) {
				board_state.push_back(std::wstring({ static_cast<wchar_t>((*it_figure)->get_description()), static_cast<wchar_t>((*it_figure)->get_pos()) }));
			}
		}
	}

	Truelog::sync_print([&]() {
		Truelog::stream() << Truelog::Type::Info << "Generated board state";

		std::string filling_of_board = "";

		for (size_t i = 0; i < m_board.size(); ++i) {
			for (size_t j = 0; j < m_board.size(); ++j) {
				Figure* current_figure = m_board[j][7 - i].get();
				if (current_figure == nullptr) {
					filling_of_board += " __";
				}
				else {
					if (current_figure->get_color() == Figure::Color::White) {
						filling_of_board += " W";
					}
					else {
						filling_of_board += " B";
					}
					switch (current_figure->get_type()) {
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

		Truelog::stream() << Truelog::Type::Debug << "Filling of board after default setup (room=" << m_room->get_name() << "): \n" << filling_of_board;
	});

	return board_state;
}
