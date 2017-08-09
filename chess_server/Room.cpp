#include "Room.h"

Room::Room() : m_is_private(false), m_is_chat_enabled(false), m_name(L""), m_owner(L""), m_white_player(L""), m_black_player(L"")
{
}

Room::Room(const std::wstring& name,
	const std::wstring& owner,
	const std::wstring& white_player,
	const std::wstring& black_player,
	bool is_private,
	bool chat_enabled) :
	m_name(name),
	m_owner(owner),
	m_white_player(white_player),
	m_black_player(black_player),
	m_is_private(is_private),
	m_is_chat_enabled(chat_enabled)
{
}

Room::~Room()
{
}

void Room::init(sqlite3*& db, std::list<std::unique_ptr<Room>>& rooms)
{
	sqlite3_stmt* statement;
	int rc = 0;

	sqlite3_prepare16_v2(db, L"SELECT * FROM rooms", -1, &statement, 0);

	while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
		std::wstring room_name(static_cast<const wchar_t*>(sqlite3_column_text16(statement, 0)));
		std::wstring room_owner(static_cast<const wchar_t*>(sqlite3_column_text16(statement, 1)));
		std::wstring room_white_player(static_cast<const wchar_t*>(sqlite3_column_text16(statement, 2)));
		std::wstring room_black_player(static_cast<const wchar_t*>(sqlite3_column_text16(statement, 3)));
		bool room_is_private = sqlite3_column_int(statement, 4);
		bool room_is_chat_enabled = sqlite3_column_int(statement, 5);

		rooms.push_back(std::make_unique<Room>(room_name, room_owner, room_white_player, room_black_player, room_is_private, room_is_chat_enabled));
	}
}

int Room::add_user(User * user)
{
	// Check already entered user in room
	for (std::list<User*>::iterator it_user = m_users.begin(); it_user != m_users.end(); it_user++) {
		if ((*it_user)->get_login() == user->get_login()) {
			return 0;
		}
	}

	

	if (user->get_login() == m_white_player) {
		user->set_role(User::Role::White_player);
	}
	else if (user->get_login() == m_black_player) {
		user->set_role(User::Role::Black_player);
	}
	else {
		user->set_role(User::Role::Spectator);
	}

	m_users.push_back(user);
	return 1;
}

int Room::erase_user(User * user)
{
	if (user->get_room() != nullptr) {
		for (std::list<User*>::iterator it = m_users.begin(); it != m_users.end(); it++) {
			if (user == (*it)) {
				m_users.erase(it);
				user->set_room(nullptr);
				user->set_role(User::Role::None);
				return 1;
			}
		}
	}
	return 0;
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

std::wstring Room::get_name() const
{
	return m_name;
}

std::wstring Room::get_owner() const
{
	return m_owner;
}

std::wstring Room::get_white_player() const
{
	return m_white_player;
}

std::wstring Room::get_black_player() const
{
	return m_black_player;
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
