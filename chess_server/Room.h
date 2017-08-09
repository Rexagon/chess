#pragma once

#include <functional>
#include <memory>
#include <vector>
#include <string>
#include <list>
#include <map>

#include <sqlite3.h>

#include "User.h"
#include "CommandPacket.h"

class Room {
public:
	Room();
	Room(const std::wstring& name,
		const std::wstring& owner,
		const std::wstring& white_player,
		const std::wstring& black_player,
		bool is_private,
		bool chat_enabled);
	~Room();

	static void init(sqlite3*& db, std::list<std::unique_ptr<Room>>& rooms);

	int add_user(User* user);
	int erase_user(User* user);
	void set_name(const std::wstring& name);
	void set_owner(const std::wstring& owner);
	void set_private(bool state);
	void set_chat_enabled(bool state);

	std::wstring get_name() const;
	std::wstring get_owner() const;
	std::wstring get_white_player() const;
	std::wstring get_black_player() const;
	bool is_private() const;
	bool is_chat_enabled() const;

	unsigned int get_spectator_count() const;

	std::list<User*>& get_users_list();
private:
	std::list<User*> m_users;

	std::wstring m_name;

	std::wstring m_owner;
	std::wstring m_white_player;
	std::wstring m_black_player;
	
	bool m_is_private;
	bool m_is_chat_enabled;
};