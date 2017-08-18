#include "Board.h"

#include "AssetManager.h"

Board::Board() :
	m_main_color(Figure::Color::White),
	m_cell_size(74.0f, 74.0f), m_board_size(700.0f, 700.0f), m_board_padding(54.0f, 54.0f),
	m_is_initialized(false)
{
	// Board sprite init
	sf::Texture* board_texture = AssetManager::get<sf::Texture>("game_chess_board");
	board_texture->setSmooth(true);

	m_board_size = vec2(board_texture->getSize());
	
	m_board_sprite.setTexture(*board_texture, true);
	m_board_sprite.setOrigin(m_board_size / 2.0f);
	
	setOrigin(m_board_size / 2.0f);

	// Figures sprites init
	sf::Texture* figures_texture = AssetManager::get<sf::Texture>("game_figures");
	figures_texture->setSmooth(true);

	vec2 figure_image_size(128.0f, 128.0f);
	vec2 figure_sprite_scale(m_cell_size.x / figure_image_size.x, m_cell_size.y / figure_image_size.y);
	for (unsigned int i = 0; i < 6; ++i) {
		for (unsigned int j = 0; j < 2; ++j) {
			recti texture_rect(i * figure_image_size.x, j * figure_image_size.y, figure_image_size.x, figure_image_size.y);
			m_figure_sprites.push_back(sf::Sprite(*figures_texture, texture_rect));
			m_figure_sprites.back().setScale(figure_sprite_scale);
		}
	}
}

Board::~Board()
{
	for (unsigned int i = 0; i < 8; ++i) {
		for (unsigned int j = 0; j < 8; ++j) {
			m_field[i][j].reset(nullptr);
		}
	}
}

void Board::init_field()
{
	for (unsigned int i = 0; i < 8; ++i) {
		for (unsigned int j = 0; j < 8; ++j) {
			m_field[i][j].reset(nullptr);
		}
	}

	for (char x = 0; x < 8; ++x) {
		set_figure(x, 1, Figure(Figure::Color::White, Figure::Type::Pawn, x));
		set_figure(x, 6, Figure(Figure::Color::Black, Figure::Type::Pawn, x));
	}

	set_figure(0, 0, Figure(Figure::Color::White, Figure::Type::Rook, 0));
	set_figure(7, 0, Figure(Figure::Color::White, Figure::Type::Rook, 1));
	set_figure(0, 7, Figure(Figure::Color::Black, Figure::Type::Rook, 0));
	set_figure(7, 7, Figure(Figure::Color::Black, Figure::Type::Rook, 1));

	set_figure(1, 0, Figure(Figure::Color::White, Figure::Type::Knight, 0));
	set_figure(6, 0, Figure(Figure::Color::White, Figure::Type::Knight, 1));
	set_figure(1, 7, Figure(Figure::Color::Black, Figure::Type::Knight, 0));
	set_figure(6, 7, Figure(Figure::Color::Black, Figure::Type::Knight, 1));

	set_figure(2, 0, Figure(Figure::Color::White, Figure::Type::Bishop, 0));
	set_figure(5, 0, Figure(Figure::Color::White, Figure::Type::Bishop, 1));
	set_figure(2, 7, Figure(Figure::Color::Black, Figure::Type::Bishop, 0));
	set_figure(5, 7, Figure(Figure::Color::Black, Figure::Type::Bishop, 1));

	set_figure(3, 0, Figure(Figure::Color::White, Figure::Type::Queen, 0));
	set_figure(4, 0, Figure(Figure::Color::White, Figure::Type::King, 0));

	set_figure(3, 7, Figure(Figure::Color::Black, Figure::Type::Queen, 0));
	set_figure(4, 7, Figure(Figure::Color::Black, Figure::Type::King, 0));

	m_is_initialized = true;
}

void Board::init_field(const std::string & field_file)
{
	// TODO: parse file

	m_is_initialized = true;
}

void Board::init_field(const CommandPacket & packet)
{
	// TODO: parse packet

	m_is_initialized = true;
}

bool Board::is_initialized() const
{
	return m_is_initialized;
}

void Board::move_figure(const vec2c& origin, const vec2c& destination)
{
	Figure* figure = get_figure(origin);
	if (figure != nullptr) {
		set_figure(destination, *figure);
		m_field[origin.x][origin.y].reset(nullptr);
	}
}

void Board::set_figure(char x, char y, const Figure & figure)
{
	if (is_position_valid(x, y)) {
		m_field[x][y] = std::make_unique<Figure>(figure);
		m_field[x][y]->set_position(x, y);
	}
}

void Board::set_figure(const vec2c& position, const Figure & figure)
{
	set_figure(position.x, position.y, figure);
}

Figure * Board::get_figure(char x, char y) const
{
	if (is_position_valid(x, y)) {
		return m_field[x][y].get();
	}
	else {
		return nullptr;
	}
}

Figure * Board::get_figure(const vec2c & position) const
{
	return get_figure(position.x, position.y);
}

void Board::set_main_color(Figure::Color color)
{
	m_main_color = color;
	if (m_main_color == Figure::Color::Black) {
		m_board_sprite.setScale(-1, -1);
	}
	else {
		m_board_sprite.setScale(1, 1);
	}
}

Figure::Color Board::get_main_color() const
{
	return m_main_color;
}

bool Board::can_select(char x, char y) const
{
	Figure* figure = get_figure(x, y);
	return (figure != nullptr) && (figure->get_color() == m_main_color);
}

bool Board::can_select(const vec2c & position) const
{
	return can_select(position.x, position.y);
}

std::vector<vec2c> Board::get_possible_moves(char x, char y) const
{
	Figure* figure = get_figure(x, y);
	if (figure == nullptr) {
		return std::vector<vec2c>();
	}

	switch (figure->get_type())
	{
	case Figure::Pawn:
		return get_moves_pawn(figure);
		break;

	case Figure::Rook:
		return get_moves_rook(figure);
		break;

	case Figure::Knight:
		return get_moves_knight(figure);
		break;

	case Figure::Bishop:
		return get_moves_bishop(figure);
		break;

	case Figure::Queen:
		return get_moves_queen(figure);
		break;

	case Figure::King:
		return get_moves_king(figure);
		break;

	default:
		return std::vector<vec2c>();
		break;
	}
}

std::vector<vec2c> Board::get_possible_moves(const vec2c & position) const
{
	return get_possible_moves(position.x, position.y);
}

void Board::draw_board(sf::RenderTarget * target)
{
	m_board_sprite.setPosition(getPosition());
	target->draw(m_board_sprite);
}

void Board::draw_figures(sf::RenderTarget * target)
{
	vec2 field_offset = getPosition() + m_board_padding - getOrigin();
	field_offset.x = ceilf(field_offset.x);
	field_offset.y = ceilf(field_offset.y);

	for (unsigned int i = 0; i < 8; ++i) {
		for (unsigned int j = 0; j < 8; ++j) {
			Figure* figure = m_field[i][j].get();
			if (figure != nullptr) {				
				int x, y;
				if (m_main_color == Figure::Color::White) {
					x = i;
					y = 7 - j;
				}
				else {
					x = 7 - i;
					y = j;					
				}

				sf::Sprite& sprite = m_figure_sprites[figure->get_type() * 2 + figure->get_color()];

				vec2 cell_position = field_offset + vec2(x * m_cell_size.x, y * m_cell_size.y);
				sprite.setPosition(cell_position);

				target->draw(sprite);
			}
		}
	}
}

vec2 Board::get_cell_size() const
{
	return m_cell_size;
}

vec2 Board::get_size() const
{
	return m_board_size;
}

vec2 Board::get_padding() const
{
	return m_board_padding;
}

bool Board::is_position_valid(char x, char y)
{
	return x >= 0 && x <= 7 && y >= 0 && y <= 7;
}

bool Board::is_position_valid(const vec2c & position)
{
	return is_position_valid(position.x, position.y);
}

std::vector<vec2c> Board::get_moves_pawn(Figure * figure) const
{
	std::vector<vec2c> moves;

	vec2c position = figure->get_position();

	vec2c diagonal_left;
	vec2c diagonal_right;
	vec2c forward;

	if (figure->get_color() == Figure::Color::White) {
		diagonal_left = position + vec2c(-1, 1);
		diagonal_right = position + vec2c(1, 1);
		forward = position + vec2c(0, 1);

		if (position.y == 1) {
			moves.push_back(position + vec2c(0, 2));
		}
	}
	else {
		diagonal_left = position - vec2c(-1, 1);
		diagonal_right = position - vec2c(1, 1);
		forward = position - vec2c(0, 1);

		if (position.y == 6) {
			moves.push_back(position - vec2c(0, 2));
		}
	}

	if (get_figure(diagonal_left) != nullptr && is_position_valid(diagonal_left) &&
		get_figure(diagonal_left)->get_color() != figure->get_color())
	{
		moves.push_back(diagonal_left);
	}

	if (get_figure(diagonal_right) != nullptr && is_position_valid(diagonal_right) &&
		get_figure(diagonal_right)->get_color() != figure->get_color())
	{
		moves.push_back(diagonal_right);
	}

	if (is_position_valid(forward) &&
		get_figure(forward) == nullptr) 
	{
		moves.push_back(forward);
	}

	return moves;
}

std::vector<vec2c> Board::get_moves_rook(Figure * figure) const
{
	std::vector<vec2c> moves;

	vec2c position = figure->get_position();

	vec2c directions[] = { 
		vec2c(1, 0), vec2c(-1, 0), vec2c(0, 1), vec2c(0, -1) 
	};

	for (int i = 0; i < 4; ++i) {
		vec2c temp = position;

		while(true) {
			temp += directions[i];

			if (!is_position_valid(temp)) {
				break;
			}

			if (get_figure(temp) == nullptr) {
				moves.push_back(temp);
			}
			else {
				if (get_figure(temp)->get_color() != m_main_color) {
					moves.push_back(temp);
				}
				break;
			}
		}
	}

	return moves;
}

std::vector<vec2c> Board::get_moves_knight(Figure * figure) const
{
	std::vector<vec2c> moves;

	vec2c position = figure->get_position();

	vec2c directions[] = {
		vec2c(1, 2), vec2c(2, 1), vec2c(2, -1), vec2c(1, -2),
		vec2c(-1, -2), vec2c(-2, -1), vec2c(-2, 1), vec2c(-1, 2)
	};

	for (int i = 0; i < 8; ++i) {
		vec2c temp = position + directions[i];

		if (!is_position_valid(temp)) {
			continue;
		}

		if (get_figure(temp) == nullptr) {
			moves.push_back(temp);
		}
		else if (get_figure(temp)->get_color() != m_main_color) {
			moves.push_back(temp);
		}
	}

	return moves;
}

std::vector<vec2c> Board::get_moves_bishop(Figure * figure) const
{
	std::vector<vec2c> moves;

	vec2c position = figure->get_position();

	vec2c directions[] = { 
		vec2c(1, 1), vec2c(1, -1), vec2c(-1, -1), vec2c(-1, 1) 
	};

	for (int i = 0; i < 4; ++i) {
		vec2c temp = position;

		while (true) {
			temp += directions[i];

			if (!is_position_valid(temp)) {
				break;
			}

			if (get_figure(temp) == nullptr) {
				moves.push_back(temp);
			}
			else {
				if (get_figure(temp)->get_color() != m_main_color) {
					moves.push_back(temp);
				}
				break;
			}
		}
	}

	return moves;
}

std::vector<vec2c> Board::get_moves_queen(Figure * figure) const
{
	std::vector<vec2c> moves;

	vec2c position = figure->get_position();

	vec2c directions[] = { 
		vec2c(1, 0), vec2c(-1, 0), vec2c(0, 1), vec2c(0, -1),
		vec2c(1, 1), vec2c(1, -1), vec2c(-1, -1), vec2c(-1, 1) 
	};

	for (int i = 0; i < 8; ++i) {
		vec2c temp = position;

		while (true) {
			temp += directions[i];

			if (!is_position_valid(temp)) {
				break;
			}

			if (get_figure(temp) == nullptr) {
				moves.push_back(temp);
			}
			else {
				if (get_figure(temp)->get_color() != m_main_color) {
					moves.push_back(temp);
				}
				break;
			}
		}
	}

	return moves;
}

std::vector<vec2c> Board::get_moves_king(Figure * figure) const
{
	std::vector<vec2c> moves;

	vec2c position = figure->get_position();

	vec2c directions[] = {
		vec2c(1, 0), vec2c(-1, 0), vec2c(0, 1), vec2c(0, -1),
		vec2c(1, 1), vec2c(1, -1), vec2c(-1, -1), vec2c(-1, 1)
	};

	for (int i = 0; i < 8; ++i) {
		vec2c temp = position + directions[i];

		if (!is_position_valid(temp)) {
			continue;
		}

		if (get_figure(temp) == nullptr) {
			moves.push_back(temp);
		}
		else if (get_figure(temp)->get_color() != m_main_color) {
			moves.push_back(temp);
		}
	}

	return moves;
}