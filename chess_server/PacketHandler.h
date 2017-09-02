#pragma once

#include <condition_variable>
#include <functional>
#include <thread>
#include <vector>
#include <mutex>
#include <queue>
#include <map>

#include <SFML\Network.hpp>
#include <sqlite3.h>

#include "CommandPacket.h"

#include "Truelog.h"
#include "SHA256.h"
#include "User.h"

class Server;

class PacketHandler {
public:
	PacketHandler(Server* server);

	~PacketHandler() {
		m_packets_handler_thread.join();
	}

	void start();
	void stop();

	void send(User* user, CommandPacket packet);

private:
	PacketHandler(PacketHandler const&) = delete;
	PacketHandler& operator= (PacketHandler const&) = delete;

	Server* m_server;

	// PacketsEmiter
	std::thread m_packets_handler_thread;

	void handler();

	// Event list
	std::map<signed char, std::function<void(User*, CommandPacket)>> m_events;
};