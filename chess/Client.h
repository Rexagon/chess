#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <map>

#include <SFML/Network.hpp>

#include "CommandPacket.h"

class Client
{
	Client();
public:
	static void init(const std::string& ip, unsigned short port);
	static void close();

	static void set_login(const std::wstring& login);
	static std::wstring get_login();

	static void send_command(const CommandPacket& packet, std::function<void(sf::Socket::Status)> callback = [](sf::Socket::Status) {});

	static void add_command_handler(signed char command, std::function<void(const CommandPacket&)> handler, bool temp = true);
	static void remove_command_handler(signed char command);
private:
	static void packets_handler();
	static void solve_command_packet(CommandPacket packet);

	static std::thread packets_handler_thread;

	static unsigned short m_port;
	static bool m_is_running;

	static std::wstring m_login;

	static sf::TcpSocket m_socket;
	static std::mutex m_socket_selector_mutex;
	static sf::SocketSelector m_socket_selector;

	static std::recursive_mutex m_command_handlers_mutex;
	static std::map<signed char, std::pair<std::function<void(const CommandPacket&)>, bool>> m_command_handlers;
};