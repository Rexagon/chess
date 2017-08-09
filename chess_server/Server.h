#pragma once

#include <exception>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <memory>
#include <list>

#include <SFML/Network.hpp>
#include <sqlite3.h>

#include "PacketHandler.h"
#include "User.h"
#include "Room.h"

class Server {
public:
	Server(unsigned short port);
	~Server() {}
	
	void run();
private:
	friend class PacketHandler;

	PacketHandler m_packet_handler;

	sf::TcpListener m_listener;
	sf::SocketSelector m_socket_selector;

	std::list<std::unique_ptr<User>> m_users;
	std::list<std::unique_ptr<Room>> m_rooms;

	unsigned short m_port;

	sqlite3* m_db;

	bool m_is_run;
};