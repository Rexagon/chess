#include "RoomManager.h"

#include "Server.h"

RoomManager::RoomManager(Server* server) : m_server(server)
{
}

RoomManager::~RoomManager()
{
}

void RoomManager::init()
{
	sqlite3_stmt* statement;
	int rc = 0;

	sqlite3_prepare16_v2(m_server->m_db, L"SELECT * FROM rooms", -1, &statement, 0);

	while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
		std::wstring room_name(static_cast<const wchar_t*>(sqlite3_column_text16(statement, 0)));
		std::wstring room_owner(static_cast<const wchar_t*>(sqlite3_column_text16(statement, 1)));
		std::wstring room_white_player(static_cast<const wchar_t*>(sqlite3_column_text16(statement, 2)));
		std::wstring room_black_player(static_cast<const wchar_t*>(sqlite3_column_text16(statement, 3)));
		bool room_is_private = static_cast<bool>(sqlite3_column_int(statement, 4));
		bool room_is_chat_enabled = static_cast<bool>(sqlite3_column_int(statement, 5));

		m_server->m_rooms.push_back(std::make_unique<Room>(room_name, room_owner, room_white_player, room_black_player, room_is_private, room_is_chat_enabled));
	}
}

Room* RoomManager::change_room_settings(const std::wstring& old_name, const std::wstring& new_name, const std::wstring & owner, const std::wstring & white_player, const std::wstring & black_player, bool privacy, bool chat_enabled)
{
	Room* room = nullptr;
	for (std::list<std::unique_ptr<Room>>::iterator it_room = m_server->m_rooms.begin(); it_room != m_server->m_rooms.end(); it_room++) {
		if ((*it_room)->get_name() == old_name) {
			room = it_room->get();
		}
	}

	std::wstring query;

	// 0 - delete; 1 - update; 2 - create
	enum Action {
		Update,
		Create
	} action = Update;

	if (room != nullptr) {
		query = L"UPDATE rooms SET (name=?, owner=?, user_white=?, user_black=?, private=?, chat_enabled=?) WHERE (name=?)";

		room->set_name(new_name);
		room->set_owner(owner);
		room->set_white_player(white_player);
		room->set_black_player(black_player);
		room->set_privacy(privacy);
		room->set_chat_enabled(chat_enabled);

		action = Action::Update;
	}
	else {
		query = L"INSERT INTO rooms (name, owner, user_white, user_black, private, chat_enabled) VALUES (?, ?, ?, ?, ?, ?)";
		
		m_server->m_rooms.push_back(std::make_unique<Room>(
			new_name,
			owner,
			white_player,
			black_player,
			privacy,
			chat_enabled
		));

		room = m_server->m_rooms.back().get();

		action = Action::Create;
	}

	sqlite3_stmt* statement;
	sqlite3_prepare16_v2(m_server->m_db, query.c_str(), -1, &statement, 0);

	sqlite3_bind_text16(statement, 1, new_name.c_str(), -1, 0);
	sqlite3_bind_text16(statement, 2, owner.c_str(), -1, 0);
	sqlite3_bind_text16(statement, 3, white_player.c_str(), -1, 0);
	sqlite3_bind_text16(statement, 4, black_player.c_str(), -1, 0);
	sqlite3_bind_int(statement, 5, static_cast<int>(privacy));
	sqlite3_bind_int(statement, 6, static_cast<int>(chat_enabled));

	if (action == Action::Update) {
		sqlite3_bind_text16(statement, 7, old_name.c_str(), -1, 0);
	}

	int rc = sqlite3_step(statement);
	sqlite3_finalize(statement);

	return room;
}

void RoomManager::erase_room(Room * room)
{
	for (std::list<std::unique_ptr<Room>>::iterator it_room = m_server->m_rooms.begin(); it_room != m_server->m_rooms.end(); it_room++) {
		if ((*it_room)->get_name() == room->get_name()) {
			m_server->m_rooms.erase(it_room);

			std::wstring query = L"DELETE FROM rooms WHERE (name=?)";

			sqlite3_stmt* statement;
			sqlite3_prepare16_v2(m_server->m_db, query.c_str(), -1, &statement, 0);

			sqlite3_bind_text16(statement, 1, room->get_name().c_str(), -1, 0);

			int rc = sqlite3_step(statement);
			sqlite3_finalize(statement);

			break;
		}
	}
}

void RoomManager::erase_room(const std::wstring & name)
{
	
}
