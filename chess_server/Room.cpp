#include "Room.h"

std::map<signed char, std::function<void(Room*, User*, const CommandPacket&)>> Room::m_handlers = {
	
};


Room::Room() : m_is_private(false), m_is_chat_enabled(false), m_name(L""), m_owner(L"")
{
}

Room::~Room()
{
}

void Room::set_name(const std::wstring & name)
{
	m_name = name;
}

void Room::set_owner(const std::wstring & owner)
{
	m_owner = owner;
}

void Room::set_private(bool state)
{
	m_is_private = state;
}

void Room::set_chat_enabled(bool state)
{
	m_is_chat_enabled = state;
}

void Room::packet_handler(User* user, const CommandPacket & packet)
{
	
}

std::wstring Room::get_name() const
{
	return m_name;
}

std::wstring Room::get_owner() const
{
	return m_owner;
}

bool Room::is_private() const
{
	return m_is_private;
}

bool Room::is_chat_enabled() const
{
	return m_is_chat_enabled;
}

unsigned int Room::get_spectator_count() const
{
	unsigned int count = 0;

	for (std::list<User*>::const_iterator it_user = m_users.begin(); it_user != m_users.end(); it_user++) {
		if ((*it_user)->get_role() == User::Role::Spectator) {
			count++;
		}
	}

	return count;
}

std::list<User*>& Room::get_users_list()
{
	return m_users;
}
