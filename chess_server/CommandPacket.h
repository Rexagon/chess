#pragma once

#include <string>
#include <map>

#include <SFML/Network.hpp>

class CommandPacket
{
public:
	enum Type {
		Signin,
		Signup,
		Signout,
		Accept,
		Reject,
		RoomList,
		ChangeRole,
		FigureMove,
		ChangeRoom,
		SendMessage,
		RoomSettings,
		BoardState,

		TypesCount
	};

	static std::map<signed char, std::wstring> packet_types;

	CommandPacket(sf::Packet packet);
	CommandPacket(signed char command, const std::vector<std::wstring>& arguments);

	CommandPacket& operator =(const CommandPacket& packet);

	sf::Packet to_sfml_packet() const;

	bool is_valid() const;

	void set_command(signed char command);
	signed char get_command() const;

	void set_arguments(const std::vector<std::wstring>& arguments);
	std::vector<std::wstring> get_arguments() const;
	size_t get_arguments_count() const;
private:
	bool m_is_valid;
	signed char m_command;
	std::vector<std::wstring> m_arguments;
};