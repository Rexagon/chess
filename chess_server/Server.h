#pragma once

#include <vector>
#include <memory>
#include <list>

#include <SFML/Network.hpp>
#include <sqlite3.h>

#include "PacketHandler.h"
#include "User.h"

class Server {
public:
	Server();
	~Server() {}
	
	void run();
private:
	friend class PacketHandler;

	PacketHandler m_packet_handler;

	sf::TcpListener m_listener;
	sf::SocketSelector m_socket_selector;

	std::list<std::unique_ptr<User>> m_users;

	bool m_is_run;
};