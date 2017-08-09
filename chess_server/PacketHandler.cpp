#include "PacketHandler.h"

#include "Server.h"
#include "User.h"
#include "Room.h"

PacketHandler::PacketHandler(Server * server) : m_server(server)
{
	m_events = {
		{
			CommandPacket::SendMessage,
			[this](User* user, CommandPacket packet) {
				Truelog::sync_print([&]() {
					Truelog::stream() << Truelog::Type::Info << "User has sent the message (ip=" << user->get_socket()->getRemoteAddress().toString() << ", login=" << user->get_login() << ", message=" << packet.get_arguments()[0] << ").";
				});
				for (std::list<User*>::iterator it_user = user->get_room()->get_users_list().begin(); it_user != user->get_room()->get_users_list().end(); it_user++) {
					send((*it_user), CommandPacket(CommandPacket::SendMessage, std::vector<std::wstring>({ packet.get_arguments()[0], user->get_login() })));
				}
			}
		},
		{
			CommandPacket::Signin,
			[this](User* user, CommandPacket packet) {
				// Check already logged user
				for (std::list<std::unique_ptr<User>>::iterator it_user = m_server->m_users.begin(); it_user != m_server->m_users.end(); it_user++) {
					if ((*it_user)->get_login() != L"" && (*it_user)->get_login() == user->get_login()) {
						send(user, CommandPacket(CommandPacket::Signin, std::vector<std::wstring>({ L"f" })));
						Truelog::sync_print([&]() {
							Truelog::stream() << Truelog::Type::Info << "User was tried sign in (ip=" << user->get_socket()->getRemoteAddress().toString() << ", login=" << packet.get_arguments()[0] << ", password=" << packet.get_arguments()[1] << ").";
						});
						return;
					}
				}

				sqlite3_stmt* statement;
				sqlite3_prepare16_v2(m_server->m_db, L"SELECT * FROM users WHERE (login=?) AND (password=?)", -1, &statement, 0);
				
				std::wstring hash_password = sha256(packet.get_arguments()[1]);

				sqlite3_bind_text16(statement, 1, packet.get_arguments()[0].c_str(), -1, 0);
				sqlite3_bind_text16(statement, 2, hash_password.c_str(), -1, 0);

				if (sqlite3_step(statement) == SQLITE_ROW) {
					send(user, CommandPacket(CommandPacket::Signin, std::vector<std::wstring>({ L"t" })));
					user->set_login(packet.get_arguments()[0]);

					Truelog::sync_print([&]() {
						std::string tmp = "";
						for (size_t i = 0; i < hash_password.size() * 2; i++) {
							tmp += reinterpret_cast<char*>(&hash_password[0])[i];
						}
						Truelog::stream() << Truelog::Type::Info << "User was signed in (ip=" << user->get_socket()->getRemoteAddress().toString() << ", login=" << packet.get_arguments()[0] << ", password=" << packet.get_arguments()[1] << ", hash_pass=" << tmp << ").";
					});
				}
				else {
					send(user, CommandPacket(CommandPacket::Signin, std::vector<std::wstring>({ L"f" })));

					Truelog::sync_print([&]() {
						Truelog::stream() << Truelog::Type::Info << "User was tried sign in (ip=" << user->get_socket()->getRemoteAddress().toString() << ", login=" << packet.get_arguments()[0] << ", password=" << packet.get_arguments()[1] << ").";
					});					
				}
				sqlite3_finalize(statement);
			}
		},

		{
			CommandPacket::Signup,
			[this](User* user, CommandPacket packet) {
				// Check login and password length
				if (packet.get_arguments()[0].size() < 4 || packet.get_arguments()[0].size() > 16 ||
					packet.get_arguments()[1].size() < 4 || packet.get_arguments()[1].size() > 16)
				{
					send(user, CommandPacket(CommandPacket::Signup, std::vector<std::wstring>({ L"f" })));

					Truelog::sync_print([&]() {
						Truelog::stream() << Truelog::Type::Info << "User was tried sign up (ip=" << user->get_socket()->getRemoteAddress().toString() << ").";
					});

					return;
				}

				// Check already signed up user
				sqlite3_stmt* statement;
				sqlite3_prepare16_v2(m_server->m_db, L"SELECT * FROM users WHERE (login=?)", -1, &statement, 0);

				sqlite3_bind_text16(statement, 1, packet.get_arguments()[0].c_str(), -1, 0);

				if (sqlite3_step(statement) == SQLITE_ROW) {
					send(user, CommandPacket(CommandPacket::Signup, std::vector<std::wstring>({ L"f" })));

					Truelog::sync_print([&]() {
						Truelog::stream() << Truelog::Type::Info << "User was tried sign up (ip=" << user->get_socket()->getRemoteAddress().toString() << ").";
					});
				}
				else {
					send(user, CommandPacket(CommandPacket::Signup, std::vector<std::wstring>({ L"t" })));

					sqlite3_stmt* insert_statement;
					sqlite3_prepare16_v2(m_server->m_db, L"INSERT INTO users (login, password) VALUES (?, ?)", -1, &insert_statement, 0);
					
					std::wstring hash_password = sha256(packet.get_arguments()[1]);

					sqlite3_bind_text16(insert_statement, 1, packet.get_arguments()[0].c_str(), -1, 0);
					sqlite3_bind_text16(insert_statement, 2, hash_password.c_str(), -1, 0);

					sqlite3_step(insert_statement);

					sqlite3_finalize(insert_statement);

					Truelog::sync_print([&]() {
						std::string tmp = "";
						for (size_t i = 0; i < hash_password.size() * 2; i++) {
							tmp += reinterpret_cast<char*>(&hash_password[0])[i];
						}
						Truelog::stream() << Truelog::Type::Info << "User was signed up (ip=" << user->get_socket()->getRemoteAddress().toString() << ", login=" << packet.get_arguments()[0] << ", password=" << packet.get_arguments()[1] << ", hash_pass=" << tmp << ").";
					});
				}
				sqlite3_finalize(statement);
			}
		},

		{
			CommandPacket::Signout,
			[this](User* user, CommandPacket packet) {
				// Erase user from his room. Send empty room for him in other words
				m_events[CommandPacket::ChangeRoom](user, CommandPacket(CommandPacket::ChangeRoom, std::vector<std::wstring>({ L"" })));
				user->set_login(L"");
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
			CommandPacket::ChangeRoom,
			[this](User* user, CommandPacket packet) {
				if (user->get_room() != nullptr) {
					user->get_room()->erase_user(user);
				}

				if (packet.get_arguments_count() > 0 && packet.get_arguments()[0].size() > 0) {
					for (std::list<std::unique_ptr<Room>>::iterator it_room = m_server->m_rooms.begin(); it_room != m_server->m_rooms.end(); it_room++) {
						if ((*it_room)->get_name() == packet.get_arguments()[0]) {
							(*it_room)->add_user(user);

							// Send to user room settings
							m_events[CommandPacket::RoomSettings](user, CommandPacket(CommandPacket::RoomSettings, std::vector<std::wstring>()));

							break;
						}
					}
				}
			}
		},

		{
			CommandPacket::RoomSettings,
			[this](User* user, CommandPacket packet) {
				CommandPacket settings_packet(CommandPacket::RoomSettings, std::vector<std::wstring>(
					{
						user->get_room()->get_name(),
						user->get_room()->get_owner(),
						user->get_room()->get_white_player(),
						user->get_room()->get_black_player(),
						std::to_wstring(static_cast<int>(user->get_room()->is_private())),
						std::to_wstring(static_cast<int>(user->get_room()->is_chat_enabled()))
					}
				));

				if (packet.get_arguments_count() == 0) {
					send(user, settings_packet);
				}
				else {
					for (std::list<User*>::iterator it_user = user->get_room()->get_users_list().begin(); it_user != user->get_room()->get_users_list().end(); it_user++) {
						send(*it_user, settings_packet);
					}
				}
			}
		},

		{
			CommandPacket::SendMessage,
			[](User* user, CommandPacket packet) {
				if (user->get_room() != nullptr) {
					for (std::list<User*>::iterator it = user->get_room()->get_users_list().begin(); it != user->get_room()->get_users_list().end(); it++) {
						(*it)->get_socket()->send(packet.to_sfml_packet());
					}
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
				for (std::list<std::unique_ptr<User>>::iterator it_user = m_server->m_users.begin(); it_user != m_server->m_users.end();) {
					if (m_server->m_socket_selector.isReady(*(*it_user)->get_socket())) {
						sf::Packet sf_packet;
						sf::Socket::Status packet_status = (*it_user)->get_socket()->receive(sf_packet);

						CommandPacket packet(sf_packet);

						Truelog::sync_print([&]() {
							Truelog::stream(Truelog::StreamType::All) << Truelog::Type::Info << "Packet was received (status=" << packet_status << ", size=" << sf_packet.getDataSize() << ")";
						});

						if (packet_status == sf::Socket::Done) {
							if (packet.is_valid()) {
								std::map<signed char, std::function<void(User*, CommandPacket)>>::iterator it_event = m_events.find(packet.get_command());

								if (it_event != m_events.end()) {
									std::thread(it_event->second, it_user->get(), packet).detach();
								}
							}

							it_user++;
						}
						else if (packet_status == sf::Socket::Disconnected) {

							Truelog::sync_print([&]() {
								Truelog::stream(Truelog::StreamType::All) << Truelog::Type::Debug << "Removing user from his room...";
							});

							// Delete user from his room
							m_events[CommandPacket::ChangeRoom](it_user->get(), CommandPacket(CommandPacket::ChangeRoom, std::vector<std::wstring>({ L"" })));

							Truelog::sync_print([&]() {
								Truelog::stream(Truelog::StreamType::All) << Truelog::Type::Debug << "Remove socket from socket selector...";
							});

							m_server->m_socket_selector.remove(*(*it_user)->get_socket());

							Truelog::sync_print([&]() {
								Truelog::stream(Truelog::StreamType::All) << Truelog::Type::Debug << "Disconnect socket...";
							});

							(*it_user)->get_socket()->disconnect();

							Truelog::sync_print([&]() {
								Truelog::stream(Truelog::StreamType::All) << Truelog::Type::Debug << "Remove user from user list...";
							});

							Truelog::sync_print([&]() {
								Truelog::stream(Truelog::StreamType::All) << Truelog::Type::Info << "User was disconnected from server (ip=" << (*it_user)->get_socket()->getRemoteAddress().toString() << ")";
							});

							it_user = m_server->m_users.erase(it_user);
						}
					}
					else {
						it_user++;
					}
				}
			}
		}
	}
}
