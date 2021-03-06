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
#include "ChessBoard.h"

class Room {
public:
	Room();
	Room(const std::wstring& name,
		const std::wstring& owner,
		const std::wstring& white_player,
		const std::wstring& black_player,
		bool is_private,
		bool chat_enabled,
		const std::wstring& file_path);
	~Room();

	int add_user(User* user);
	int erase_user(User* user);
	void set_name(const std::wstring& name);
	void set_owner(const std::wstring& owner);
	void set_white_player(const std::wstring& white_player);
	void set_black_player(const std::wstring& black_player);
	void set_privacy(bool privacy);
	void set_chat_enabled(bool chat_enabled);

	std::wstring get_name() const;
	std::wstring get_owner() const;
	std::wstring get_white_player() const;
	std::wstring get_black_player() const;
	bool is_private() const;
	bool is_chat_enabled() const;

	unsigned int get_spectator_count() const;

	std::list<User*>& get_users_list();

	ChessBoard* get_chess_board();
	std::wstring get_file_path() const;
private:
	std::list<User*> m_users;

	ChessBoard m_chess_board;

	std::wstring m_name;
	std::wstring m_file_path;

	std::wstring m_owner;
	std::wstring m_white_player;
	std::wstring m_black_player;
	
	bool m_privacy;
	bool m_is_chat_enabled;
};