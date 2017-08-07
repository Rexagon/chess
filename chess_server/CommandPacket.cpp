#include "CommandPacket.h"

CommandPacket::CommandPacket(sf::Packet packet)
{
	m_is_valid = packet >> m_command;

	std::string argument;
	while (packet >> argument) {
		m_arguments.push_back(argument);
	}
}

CommandPacket::CommandPacket(signed char command, const std::vector<std::string>& arguments) :
	m_command(command), m_arguments(arguments)
{
}

sf::Packet CommandPacket::to_sfml_packet() const
{
	sf::Packet packet;
	packet << m_command;
	for (const auto& argument : m_arguments) {
		packet << argument;
	}
	return packet;
}

bool CommandPacket::is_valid() const
{
	return m_is_valid;
}

void CommandPacket::set_command(signed char command)
{
	m_command = command;
}

signed char CommandPacket::get_command() const
{
	return m_command;
}

void CommandPacket::set_arguments(const std::vector<std::string>& arguments)
{
	m_arguments = arguments;
}

std::vector<std::string> CommandPacket::get_arguments() const
{
	return m_arguments;
}

unsigned int CommandPacket::get_arguments_count() const
{
	return m_arguments.size();
}
