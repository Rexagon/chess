#include "Client.h"

#include "Log.h"

std::thread Client::packets_handler_thread;

unsigned short Client::m_port;
bool Client::m_is_running;

std::wstring Client::m_login;

sf::TcpSocket Client::m_socket;
std::mutex Client::m_socket_selector_mutex;
sf::SocketSelector Client::m_socket_selector;

std::recursive_mutex Client::m_command_handlers_mutex;
std::map<signed char, std::pair<std::function<void(const CommandPacket&)>, bool>> Client::m_command_handlers;


void Client::init(const std::string& ip, unsigned short port)
{
	m_port = port;
	if (m_socket.connect(sf::IpAddress(ip), m_port) != sf::Socket::Done) {
		throw std::runtime_error("unable to connect to localhost:" + std::to_string(m_port));
	}

	m_socket_selector.add(m_socket);

	m_is_running = true;
	std::unique_lock<std::mutex> lock(m_socket_selector_mutex);
	packets_handler_thread = std::thread(&Client::packets_handler);
	packets_handler_thread.detach();

	Log::write("successfully connected to server");
}

void Client::close()
{
	m_is_running = false;
	std::unique_lock<std::mutex> lock(m_socket_selector_mutex);
}

void Client::set_login(const std::wstring & login)
{
	m_login = login;
}

std::wstring Client::get_login()
{
	return m_login;
}

void Client::send_command(const CommandPacket & packet, std::function<void(sf::Socket::Status)> callback)
{
	std::thread([](sf::Packet packet, std::function<void(sf::Socket::Status)> callback) {
		callback(m_socket.send(packet));
	}, packet.to_sfml_packet(), callback).detach();
}

void Client::add_command_handler(signed char command, std::function<void(const CommandPacket&)> handler, bool temp)
{
	std::unique_lock<std::recursive_mutex> lock(m_command_handlers_mutex);
	m_command_handlers[command] = std::make_pair(handler, temp);
}

void Client::remove_command_handler(signed char command)
{
	std::unique_lock<std::recursive_mutex> lock(m_command_handlers_mutex);
	auto it = m_command_handlers.find(command);
	if (it != m_command_handlers.end()) {
		m_command_handlers.erase(it);
	}
}

void Client::packets_handler()
{
	while (m_is_running) {
		std::unique_lock<std::mutex> lock(m_socket_selector_mutex);
		if (m_socket_selector.wait(sf::seconds(1.0f))) {
			if (m_socket_selector.isReady(m_socket)) {
				sf::Packet packet;
				sf::Socket::Status status = m_socket.receive(packet);
				if (status == sf::Socket::Done) {
					Log::write("got", packet.getDataSize(), "bytes from server");
					std::thread(&Client::solve_command_packet, CommandPacket(packet)).detach();
				}
				else if (status == sf::Socket::Disconnected) {
					Log::write("server closed the connection");
					return;
				}
				else {
					Log::write("WARNING: unable to receive packet");
				}
			}
		}
	}
}

void Client::solve_command_packet(CommandPacket packet)
{
	std::unique_lock<std::recursive_mutex> lock(m_command_handlers_mutex);
	auto it = m_command_handlers.find(packet.get_command());
	if (it != m_command_handlers.end()) {
		it->second.first(packet);
		if (it->second.second == true) {
			m_command_handlers.erase(it);
		}
	}
}
