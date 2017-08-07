#pragma once

#include <memory>

#include <SFML/Network.hpp>

class Room;

class User {
public:
	User(std::shared_ptr<sf::TcpSocket> socket);

	void set_room(Room* room);

	sf::TcpSocket* get_socket() const;

	Room* get_room() const;
private:
	std::shared_ptr<sf::TcpSocket> m_socket;

	Room* m_room;
};