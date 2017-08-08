#include "Server.h"

Server::Server(unsigned short port) : m_is_run(true), m_port(port), m_packet_handler(this)
{
}

void Server::run()
{
	// If server not open with the port throw exception
	if (m_listener.listen(m_port) != sf::Socket::Done) {
		throw std::exception(std::string("Unable to create server on port " + std::to_string(m_port)).c_str());
	}

	m_packet_handler.start();

	// Main server loop
	while (m_is_run) {

	}

	m_packet_handler.stop();
}
