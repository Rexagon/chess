#include "User.h"

#include "Room.h"

User::User(std::shared_ptr<sf::TcpSocket> socket) : m_socket(socket), m_room(nullptr), m_role(Role::None), m_login(L"")
{
}

void User::set_room(Room* room)
{
	m_room = room;
}

void User::set_role(const Role & role)
{
	m_role = role;
}

void User::set_login(const std::wstring & login)
{
	m_login = login;
}

sf::TcpSocket * User::get_socket() const
{
	return m_socket.get();
}

Room * User::get_room() const
{
	return m_room;
}

const User::Role User::get_role() const
{
	return m_role;
}

const std::wstring User::get_login() const
{
	return m_login;
}
