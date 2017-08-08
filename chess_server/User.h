#pragma once

#include <memory>
#include <string>

#include <SFML/Network.hpp>

class Room;

class User {
public:
	// No racism, only business
	enum class Role {
		None = -1,
		Spectator,
		White_player,
		Black_player
	};

	User(std::shared_ptr<sf::TcpSocket> socket);

	void set_room(Room* room);
	void set_role(const Role& role);
	void set_login(const std::wstring& login);

	sf::TcpSocket* get_socket() const;
	Room* get_room() const;
	const Role get_role() const;
	const std::wstring get_login() const;

private:
	std::shared_ptr<sf::TcpSocket> m_socket;

	std::wstring m_login;

	Room* m_room;
	Role m_role;
};