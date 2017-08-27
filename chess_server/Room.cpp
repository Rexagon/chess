#include "Room.h"

Room::Room() : m_privacy(false), m_is_chat_enabled(false), m_name(L""), m_owner(L""), m_white_player(L""), m_black_player(L""), m_chess_board(this)
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
	m_privacy(is_private),
	m_is_chat_enabled(chat_enabled),
	m_chess_board(this)
{
}

Room::~Room()
{
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

	user->set_room(this);
	m_users.push_back(user);

	return 1;
}

int Room::erase_user(User * user)
{
	if (user->get_room() != nullptr) {
		for (std::list<User*>::iterator it_user = m_users.begin(); it_user != m_users.end(); it_user++) {
			if (user->get_login() == (*it_user)->get_login()) {
				m_users.erase(it_user);
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

void Room::set_white_player(const std::wstring & white_player)
{
	m_white_player = white_player;
}

void Room::set_black_player(const std::wstring & black_player)
{
	m_black_player = black_player;
}

void Room::set_privacy(bool privacy)
{
	m_privacy = privacy;
}

void Room::set_chat_enabled(bool chat_enabled)
{
	m_is_chat_enabled = chat_enabled;
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
	return m_privacy;
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

ChessBoard * Room::get_chess_board()
{
	return &m_chess_board;
}
