#pragma once

#include <memory>
#include <array>

#include <SFML/Graphics.hpp>

#include "CommandPacket.h"
#include "Figure.h"

class Board : public sf::Transformable
{
public:
	Board();
	~Board();

	void init_field();
	void init_field(const std::string& field_file);
	void init_field(const CommandPacket& packet);

	bool is_initialized() const;

	void move_figure(const vec2c& origin, const vec2c& destination);
	void set_figure(char x, char y, const Figure& figure);
	void set_figure(const vec2c& position, const Figure& figure);
	Figure* get_figure(char x, char y) const;
	Figure* get_figure(const vec2c& position) const;

	void set_main_color(Figure::Color color);
	Figure::Color get_main_color() const;

	bool can_select(char x, char y) const;
	bool can_select(const vec2c& position) const;
	std::vector<vec2c> get_possible_moves(char x, char y) const;
	std::vector<vec2c> get_possible_moves(const vec2c& position) const;

	void draw_board(sf::RenderTarget* target);
	void draw_figures(sf::RenderTarget* target);

	vec2 get_cell_size() const;

	vec2 get_size() const;
	vec2 get_padding() const;

	static bool is_position_valid(char x, char y);
	static bool is_position_valid(const vec2c& position);

private:
	std::vector<vec2c> get_moves_pawn(Figure* figure) const;
	std::vector<vec2c> get_moves_rook(Figure* figure) const;
	std::vector<vec2c> get_moves_knight(Figure* figure) const;
	std::vector<vec2c> get_moves_bishop(Figure* figure) const;
	std::vector<vec2c> get_moves_queen(Figure* figure) const;
	std::vector<vec2c> get_moves_king(Figure* figure) const;
		
	Figure::Color m_main_color;

	std::array<std::array<std::unique_ptr<Figure>, 8>, 8> m_field;

	vec2 m_cell_size;
	vec2 m_board_size;
	vec2 m_board_padding;

	sf::Sprite m_board_sprite;
	sf::Sprite m_board_shadow_sprite;
	std::vector<sf::Sprite> m_figure_sprites;

	bool m_is_initialized;
};