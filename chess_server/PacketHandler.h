#pragma once

#include <condition_variable>
#include <functional>
#include <thread>
#include <vector>
#include <mutex>
#include <queue>

#include <SFML\Network.hpp>

#include "CommandPacket.h"

#include "Truelog.h"
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

	std::pair<User*, sf::Packet>* pull_packet();

	void send(User* user, sf::Packet packet);

	const sf::Socket::Status get_status() const;

	// Return state of queue:
	// return false if queue is empty, otherwise return true
	bool has_packets();

	// Registered event for the type
	void single_packet_event(sf::Int8 type, std::function<void(sf::Packet*)> callback);

private:
	PacketHandler(PacketHandler const&) = delete;
	PacketHandler& operator= (PacketHandler const&) = delete;

	Server* m_server;

	// PacketsEmiter
	std::thread m_packets_handler_thread;
	std::mutex m_handler_mutex;
	std::condition_variable m_condition_handling;

	sf::Socket::Status m_status;

	// Add packet into queue and if event list is not empty
	// compare event type with packet type

	void Handler();

	std::queue<std::pair<User*, sf::Packet>> m_packets_queue;
	std::pair<User*, sf::Packet> m_packet_back;
	std::mutex m_queue_mutex;

	// Event list
	std::vector<std::pair<sf::Int8, std::function<void(sf::Packet*)>>> m_events;
	std::mutex m_events_mutex;

	// Condition variable
	bool m_is_run;
};