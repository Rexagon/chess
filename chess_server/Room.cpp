#include "Room.h"

std::map<signed char, std::function<void(Room*, User*, const CommandPacket&)>> Room::m_handlers = {
	{ 
		CommandPacket::EnterRoom,
		[](Room* room, User* user, const CommandPacket& packet) {
			user->set_role(User::Role::Spectator);
			room->get_users_list().push_back(user);
		} 
	},

	{
		CommandPacket::LeaveRoom,
		[](Room* room, User* user, const CommandPacket& packet) {
			for (std::list<User*>::iterator it = room->get_users_list().begin(); it != room->get_users_list().end(); it++) {
				if (user == (*it)) {
					room->get_users_list().erase(it);
					break;
				}
			}
		}
	},

	{
		CommandPacket::SendMessage,
		[](Room* room, User* user, const CommandPacket& packet) {
			for (std::list<User*>::iterator it = room->get_users_list().begin(); it != room->get_users_list().end(); it++) {
				(*it)->get_socket()->send(packet.to_sfml_packet());
			}
		}
	}
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

std::list<User*>& Room::get_users_list()
{
	return m_users;
}
