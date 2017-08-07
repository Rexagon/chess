#include "User.h"

#include "Room.h"

User::User(std::shared_ptr<sf::TcpSocket> socket) : m_socket(socket), m_room(nullptr)
{
}

void User::set_room(Room* room)
{
	m_room = room;
}

sf::TcpSocket * User::get_socket() const
{
	return m_socket.get();
}

Room * User::get_room() const
{
	return m_room;
}
