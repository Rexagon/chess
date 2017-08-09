#include "PacketHandler.h"

#include "Server.h"
#include "User.h"
#include "Room.h"

PacketHandler::PacketHandler(Server * server) : m_server(server)
{
	m_events = {
		{
			CommandPacket::Login,
			[this](User* user, CommandPacket packet) {
				for (std::list<std::unique_ptr<User>>::iterator it_users = m_server->m_users.begin(); it_users != m_server->m_users.end(); it_users++) {
					if ((*it_users)->get_login() == user->get_login()) {
						send(user, CommandPacket(CommandPacket::Reject, std::vector<std::wstring>()));
						return;
					}
				}

				sqlite3_stmt* statement;
				sqlite3_prepare16_v2(m_server->m_db, "SELECT * FROM users WHERE (login=?) AND (password=?)", -1, &statement, 0);
				
				sqlite3_bind_text16(statement, 1, packet.get_arguments()[0].c_str(), -1, 0);
				sqlite3_bind_text16(statement, 2, packet.get_arguments()[1].c_str(), -1, 0);

				if (sqlite3_step(statement) == SQLITE_ROW) {
					send(user, CommandPacket(CommandPacket::Reject, std::vector<std::wstring>()));
				}
				else {
					send(user, CommandPacket(CommandPacket::Accept, std::vector<std::wstring>()));
				}
			}
		},

		{
			CommandPacket::RoomList,
			[this](User* user, CommandPacket packet) {
				std::vector<std::wstring> room_list;
				size_t current_beginning = 0;
				for (std::list<std::unique_ptr<Room>>::iterator it_room = m_server->m_rooms.begin(); it_room != m_server->m_rooms.end(); it_room++) {
					// 4 elements have reserved
					for (int i = 0; i < 4; i++) {
						room_list.push_back(std::wstring(L""));
					}

					room_list[current_beginning] = (*it_room)->get_name();

					// Add white and black player login, if white or black player is empty then string is empty
					for (std::list<User*>::iterator it_room_user = (*it_room)->get_users_list().begin(); it_room_user != (*it_room)->get_users_list().end(); it_room_user++) {
						if ((*it_room_user)->get_role() == User::Role::White_player) {
							room_list[current_beginning + 1] = (*it_room_user)->get_login();
						}
						else if ((*it_room_user)->get_role() == User::Role::Black_player) {
							room_list[current_beginning + 2] = (*it_room_user)->get_login();
						}
					}

					room_list[current_beginning + 3] = std::to_wstring((*it_room)->get_spectator_count());

					current_beginning += 4;
				}
				send(user, CommandPacket(CommandPacket::RoomList, room_list));
			}
		},

		{
			CommandPacket::EnterRoom,
			[](User* user, CommandPacket packet) {
				user->set_role(User::Role::Spectator);
				user->get_room()->get_users_list().push_back(user);
			}
		},

		{
			CommandPacket::LeaveRoom,
			[](User* user, CommandPacket packet) {
				for (std::list<User*>::iterator it = user->get_room()->get_users_list().begin(); it != user->get_room()->get_users_list().end(); it++) {
					if (user == (*it)) {
						user->get_room()->get_users_list().erase(it);
						break;
					}
				}
			}
		},

		{
			CommandPacket::SendMessage,
			[](User* user, CommandPacket packet) {
				for (std::list<User*>::iterator it = user->get_room()->get_users_list().begin(); it != user->get_room()->get_users_list().end(); it++) {
					(*it)->get_socket()->send(packet.to_sfml_packet());
				}
			}
		}
	};
}

void PacketHandler::start()
{
	m_packets_handler_thread = std::thread(&PacketHandler::handler, this);

	Truelog::stream(Truelog::StreamType::File) << Truelog::Type::Info << "Start PacketHandler";

	m_server->m_socket_selector.add(m_server->m_listener);
}

void PacketHandler::stop()
{
	Truelog::stream(Truelog::StreamType::File) << Truelog::Type::Info << "Stop PacketHandler";

	m_server->m_socket_selector.clear();
}

void PacketHandler::send(User* user, CommandPacket packet)
{
	send(user, packet.to_sfml_packet());
}

void PacketHandler::send(User * user, sf::Packet packet)
{
	sf::Socket::Status status = user->get_socket()->send(packet);

	Truelog::sync_print([&]() {
		Truelog::stream(Truelog::StreamType::All) << Truelog::Type::Info << "Packet was sent to server (status=" << status << ")";
	});
}

void PacketHandler::handler()
{
	while (m_server->m_is_run) {
		if (m_server->m_socket_selector.wait(sf::milliseconds(5))) {
			if (m_server->m_socket_selector.isReady(m_server->m_listener)) {
				std::shared_ptr<sf::TcpSocket> userSocket = std::make_shared<sf::TcpSocket>();
				if (m_server->m_listener.accept(*userSocket) == sf::Socket::Done) {

					Truelog::sync_print([&]() {
						Truelog::stream(Truelog::StreamType::All) << Truelog::Type::Info << "User was connected to server (ip=" << userSocket->getRemoteAddress().toString() << ")";
					});

					m_server->m_users.push_back(std::unique_ptr<User>(new User(userSocket)));

					m_server->m_socket_selector.add(*m_server->m_users.back()->get_socket());
				}
			}
			else {
				for (std::list<std::unique_ptr<User>>::iterator it_users = m_server->m_users.begin(); it_users != m_server->m_users.end();) {
					if (m_server->m_socket_selector.isReady(*(*it_users)->get_socket())) {
						sf::Packet sf_packet;
						sf::Socket::Status packet_status = (*it_users)->get_socket()->receive(sf_packet);

						CommandPacket packet(sf_packet);

						Truelog::sync_print([&]() {
							Truelog::stream(Truelog::StreamType::All) << Truelog::Type::Info << "Packet was received (status=" << packet_status << ", size=" << sf_packet.getDataSize() << ")";
						});

						if (packet_status == sf::Socket::Done) {
							if (packet.is_valid()) {
								std::map<signed char, std::function<void(User*, CommandPacket)>>::iterator it_event = m_events.find(packet.get_command());

								if (it_event != m_events.end()) {
									std::thread(it_event->second, it_users->get(), packet).detach();
								}
							}

							it_users++;
						}
						else if (packet_status == sf::Socket::Disconnected) {

							Truelog::sync_print([&]() {
								Truelog::stream(Truelog::StreamType::All) << Truelog::Type::Debug << "Removing user from his room...";
							});

							if ((*it_users)->get_room() != nullptr) {
								m_events[CommandPacket::Type::LeaveRoom](it_users->get(), packet);
							}

							Truelog::sync_print([&]() {
								Truelog::stream(Truelog::StreamType::All) << Truelog::Type::Debug << "Remove socket from socket selector...";
							});

							m_server->m_socket_selector.remove(*(*it_users)->get_socket());

							Truelog::sync_print([&]() {
								Truelog::stream(Truelog::StreamType::All) << Truelog::Type::Debug << "Disconnect socket...";
							});

							(*it_users)->get_socket()->disconnect();

							Truelog::sync_print([&]() {
								Truelog::stream(Truelog::StreamType::All) << Truelog::Type::Debug << "Remove user from user list...";
							});

							Truelog::sync_print([&]() {
								Truelog::stream(Truelog::StreamType::All) << Truelog::Type::Info << "User was disconnected from server (ip=" << (*it_users)->get_socket()->getRemoteAddress().toString() << ")";
							});

							it_users = m_server->m_users.erase(it_users);
						}
					}
					else {
						it_users++;
					}
				}
			}
		}
	}
}
