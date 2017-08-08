#pragma once

#include <string>

#include <SFML/Network.hpp>

enum Command {
	Login,
	Logout,
	EnterRoom,
	LeaveRoom,
	SendMessage,
	UserConnected,
	UserDisconnected
};

class CommandPacket
{
public:
	CommandPacket(sf::Packet packet);
	CommandPacket(signed char command, const std::vector<std::string>& arguments);

	sf::Packet to_sfml_packet() const;

	bool is_valid() const;

	void set_command(signed char command);
	signed char get_command() const;

	void set_arguments(const std::vector<std::string>& arguments);
	std::vector<std::string> get_arguments() const;
	unsigned int get_arguments_count() const;
private:
	bool m_is_valid;
	signed char m_command;
	std::vector<std::string> m_arguments;
};