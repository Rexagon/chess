#pragma once

#include <functional>
#include <memory>
#include <vector>
#include <string>
#include <list>
#include <map>

#include "User.h"
#include "CommandPacket.h"

class Room {
public:
	Room();
	~Room();

	void set_name(const std::wstring& name);
	void set_owner(const std::wstring& owner);
	void set_private(bool state);
	void set_chat_enabled(bool state);

	void packet_handler(User* user, const CommandPacket& packet);

	std::wstring get_name() const;
	std::wstring get_owner() const;
	bool is_private() const;
	bool is_chat_enabled() const;

	std::list<User*>& get_users_list();
private:
	std::list<User*> m_users;

	std::wstring m_name;
	std::wstring m_owner;

	bool m_is_private;
	bool m_is_chat_enabled;

	static std::map<signed char, std::function<void(Room*, User*, const CommandPacket&)>> m_handlers;
};