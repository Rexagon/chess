#include "CommandPacket.h"

std::map<signed char, std::wstring> CommandPacket::packet_types = {

	{
		CommandPacket::Signin,
		L"Signin"
	},

	{
		CommandPacket::Signup,
		L"Signup"
	},

	{
		CommandPacket::Signout,
		L"Signout"
	},

	{
		CommandPacket::Accept,
		L"Accept"
	},

	{
		CommandPacket::Reject,
		L"Reject"
	},

	{
		CommandPacket::RoomList,
		L"RoomList"
	},

	{
		CommandPacket::ChangeRole,
		L"ChangeRole"
	},

	{
		CommandPacket::FigureMove,
		L"FigureMove"
	},

	{
		CommandPacket::ChangeRoom,
		L"ChangeRoom"
	},

	{
		CommandPacket::SendMessage,
		L"SendMessage"
	},

	{
		CommandPacket::RoomSettings,
		L"RoomSettings"
	},

	{
		CommandPacket::BoardState,
		L"BoardState"
	}
};

CommandPacket::CommandPacket(sf::Packet packet)
{
	m_is_valid = packet >> m_command;

	std::wstring argument;
	while (packet >> argument) {
		m_arguments.push_back(argument);
	}

	m_is_valid &= packet.endOfPacket();
}

CommandPacket::CommandPacket(signed char command, const std::vector<std::wstring>& arguments) :
	m_command(command), m_arguments(arguments)
{
}

CommandPacket & CommandPacket::operator =(const CommandPacket & packet)
{
	this->m_is_valid = packet.m_is_valid;
	this->m_command = packet.m_command;
	this->m_arguments = packet.m_arguments;

	return *this;
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

void CommandPacket::set_arguments(const std::vector<std::wstring>& arguments)
{
	m_arguments = arguments;
}

std::vector<std::wstring> CommandPacket::get_arguments() const
{
	return m_arguments;
}

size_t CommandPacket::get_arguments_count() const
{
	return m_arguments.size();
}
