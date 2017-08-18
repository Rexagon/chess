#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <list>

#include "Room.h"

class Server;

class RoomManager {
public:
	RoomManager(Server* server);
	~RoomManager();

	void init();

	Room* change_room_settings(const std::wstring& old_name,
		const std::wstring& new_name,
		const std::wstring& owner,
		const std::wstring& white_player,
		const std::wstring& black_player,
		bool privacy,
		bool chat_enabled);
	void erase_room(Room* room);
	void erase_room(const std::wstring& name);
private:
	Server* m_server;
};