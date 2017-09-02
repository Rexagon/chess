#include "PacketHandler.h"

#include "Server.h"
#include "User.h"
#include "Room.h"

PacketHandler::PacketHandler(Server * server) : m_server(server)
{
	m_events = {
		{
			CommandPacket::Signin,
			[this](User* user, CommandPacket packet) {
				// Check argument count
				if (packet.get_arguments_count() != 2) {
					send(user, CommandPacket(CommandPacket::Signin, std::vector<std::wstring>({ L"0" })));
					Truelog::sync_print([&]() {
						Truelog::stream() << Truelog::Type::Info << "User tried sign in (ip=" << user->get_socket()->getRemoteAddress().toString() << ", login=" << packet.get_arguments()[0] << ", password=" << packet.get_arguments()[1] << ").";
					});
				}

				// Check already logged user
				for (std::list<std::unique_ptr<User>>::iterator it_user = m_server->m_users.begin(); it_user != m_server->m_users.end(); it_user++) {
					if ((*it_user)->get_login() != L"" && ((*it_user)->get_login() == user->get_login() ||
						packet.get_arguments()[0] == (*it_user)->get_login()))
					{
						send(user, CommandPacket(CommandPacket::Signin, std::vector<std::wstring>({ L"0" })));
						Truelog::sync_print([&]() {
							Truelog::stream() << Truelog::Type::Info << "User tried sign in (ip=" << user->get_socket()->getRemoteAddress().toString() << ", login=" << packet.get_arguments()[0] << ", password=" << packet.get_arguments()[1] << ").";
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
					send(user, CommandPacket(CommandPacket::Signin, std::vector<std::wstring>({ L"1" })));
					user->set_login(packet.get_arguments()[0]);

					Truelog::sync_print([&]() {
						std::string tmp = "";
						for (size_t i = 0; i < hash_password.size() * 2; i++) {
							tmp += reinterpret_cast<char*>(&hash_password[0])[i];
						}
						Truelog::stream() << Truelog::Type::Info << "User signed in (ip=" << user->get_socket()->getRemoteAddress().toString() << ", login=" << packet.get_arguments()[0] << ", password=" << packet.get_arguments()[1] << ", hash_pass=" << tmp << ").";
					});
				}
				else {
					send(user, CommandPacket(CommandPacket::Signin, std::vector<std::wstring>({ L"0" })));

					Truelog::sync_print([&]() {
						Truelog::stream() << Truelog::Type::Info << "User tried sign in (ip=" << user->get_socket()->getRemoteAddress().toString() << ", login=" << packet.get_arguments()[0] << ", password=" << packet.get_arguments()[1] << ").";
					});
				}
				sqlite3_finalize(statement);
			}
		},

		{
			CommandPacket::Signup,
			[this](User* user, CommandPacket packet) {
				// Check argument count
				if (packet.get_arguments_count() != 2) {
					send(user, CommandPacket(CommandPacket::Signup, std::vector<std::wstring>({ L"0" })));

					Truelog::sync_print([&]() {
						Truelog::stream() << Truelog::Type::Info << "User tried sign up (ip=" << user->get_socket()->getRemoteAddress().toString() << ").";
					});
					return;
				}

				// Check login and password length
				if (packet.get_arguments()[0].size() < 4 || packet.get_arguments()[0].size() > 16 ||
					packet.get_arguments()[1].size() < 4 || packet.get_arguments()[1].size() > 16)
				{
					send(user, CommandPacket(CommandPacket::Signup, std::vector<std::wstring>({ L"0" })));

					Truelog::sync_print([&]() {
						Truelog::stream() << Truelog::Type::Info << "User tried sign up (ip=" << user->get_socket()->getRemoteAddress().toString() << ").";
					});

					return;
				}

				// Check already signed up user
				sqlite3_stmt* statement;
				sqlite3_prepare16_v2(m_server->m_db, L"SELECT * FROM users WHERE (login=?)", -1, &statement, 0);

				sqlite3_bind_text16(statement, 1, packet.get_arguments()[0].c_str(), -1, 0);

				if (sqlite3_step(statement) == SQLITE_ROW) {
					send(user, CommandPacket(CommandPacket::Signup, std::vector<std::wstring>({ L"0" })));

					Truelog::sync_print([&]() {
						Truelog::stream() << Truelog::Type::Info << "User tried sign up (ip=" << user->get_socket()->getRemoteAddress().toString() << ").";
					});
				}
				else {
					send(user, CommandPacket(CommandPacket::Signup, std::vector<std::wstring>({ L"1" })));

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
						Truelog::stream() << Truelog::Type::Info << "User signed up (ip=" << user->get_socket()->getRemoteAddress().toString() << ", login=" << packet.get_arguments()[0] << ", password=" << packet.get_arguments()[1] << ", hash_pass=" << tmp << ").";
					});
				}
				sqlite3_finalize(statement);
			}
		},

		{
			CommandPacket::Signout,
			[this](User* user, CommandPacket packet) {
				// Erase user from his room. Send empty room for him in other words
				m_events[CommandPacket::ChangeRoom](user, CommandPacket(CommandPacket::ChangeRoom, std::vector<std::wstring>()));
				std::wstring print_login = user->get_login();
				user->set_login(L"");
				Truelog::sync_print([&]() {
					Truelog::stream() << Truelog::Type::Info << "User signed out (ip=" << user->get_socket()->getRemoteAddress().toString() << ", login=" << print_login << ")";
				});
			}
		},

		{
			CommandPacket::RoomList,
			[this](User* user, CommandPacket packet) {
				std::vector<std::wstring> room_list;
				for (std::list<std::unique_ptr<Room>>::iterator it_room = m_server->m_rooms.begin(); it_room != m_server->m_rooms.end(); it_room++) {
					room_list.push_back((*it_room)->get_name());
					room_list.push_back((*it_room)->get_white_player());
					room_list.push_back((*it_room)->get_black_player());
					room_list.push_back(std::to_wstring((*it_room)->get_spectator_count()));
				}

				Truelog::sync_print([&]() {
					std::wstring print_packet = L"";
					int room_count = 1;
					for (size_t i = 0; i < room_list.size(); i += 4) {
						print_packet += (L"\n    // " + std::to_wstring(room_count));
						print_packet += (L"\n    room_name       = " + room_list[i + 0]);
						print_packet += (L"\n    white_player    = " + room_list[i + 1]);
						print_packet += (L"\n    black_player    = " + room_list[i + 2]);
						print_packet += (L"\n    spectator_count = " + room_list[i + 3]);
						/*
						rooms_out += L"room_name = " + room_list[i + 0] + L"\n";
						rooms_out += L"white_player = " + room_list[i + 1] + L"\n";
						rooms_out += L"white_player_enabled = " + room_list[i + 2] + L"\n";
						rooms_out += L"black_player = " + room_list[i + 3] + L"\n";
						rooms_out += L"black_player_enabled = " + room_list[i + 4] + L"\n";
						rooms_out += L"spectator_count = " + room_list[i + 5] + L"\n";
						*/
						room_count++;
					}
					Truelog::stream() << Truelog::Type::Info << "Room list prepared to user (login=" << user->get_login() << ")"
						<< "\n{"
						<< print_packet
						<< "\n}";
				});

				send(user, CommandPacket(CommandPacket::RoomList, room_list));
			}
		},
		
		/*
		** ==== Change room ====
		** 
		** If argument count is zero and if user is in the room - erase him from the room:
		** 
		** -- REQUEST --
		** {
		** }
		** -- RESPONSE --
		** {
		** }
		** 
		** Else if argument count is one then erase user from his room and add to request room:
		** 
		** -- REQUEST --
		** {
		**     room_name
		** }
		** -- RESPONSE --
		** {
		**     room_name
		** }
		** or ignore
		** 
		** Else if room owner == user login then erase all request users and send them empty change room packet:
		**
		** -- REQUEST --
		** {
		**     room_name,
		**     user1,
		**     ...,
		**     userN
		** }
		** -- RESPONSE --
		** {
		** }
		**/
		{
			CommandPacket::ChangeRoom,
			[this](User* user, CommandPacket packet) {
				// If packet is empty then erase user from his room, if he is in room
				if (packet.get_arguments_count() == 0) {
					if (user->get_room() != nullptr) {
						// Save room name for Truelog
						std::wstring room_name = user->get_room()->get_name();

						user->get_room()->erase_user(user);
						send(user, CommandPacket(CommandPacket::ChangeRoom, std::vector<std::wstring>()));

						Truelog::sync_print([&]() {
							Truelog::stream() << Truelog::Type::Info << "User signed out from his room (login=" << user->get_login() << ", room=" << room_name << ")";
						});
					}
				}
				else if (packet.get_arguments_count() == 1) {
					// Check if logged user
					if (user->get_login().size() > 0) {
						for (std::list<std::unique_ptr<Room>>::iterator it_room = m_server->m_rooms.begin(); it_room != m_server->m_rooms.end(); it_room++) {
							if ((*it_room)->get_name() == packet.get_arguments()[0]) {
								if (user->get_room() != nullptr) {
									user->get_room()->erase_user(user);
								}
								int res = (*it_room)->add_user(user);

								// Check already entered user in room
								if (res == 1) {
									// Send accept to user
									send(user, CommandPacket(CommandPacket::ChangeRoom, std::vector<std::wstring>({ (*it_room)->get_name() })));
									Truelog::sync_print([&]() {
										Truelog::stream() << Truelog::Type::Info << "User changed room (login=" << user->get_login() << ", room=" << (*it_room)->get_name() << ")";
									});
								}
								else if (res == 0) {
									send(user, CommandPacket(CommandPacket::ChangeRoom, std::vector<std::wstring>()));
									Truelog::sync_print([&]() {
										Truelog::stream() << Truelog::Type::Info << "User tried change room (login=" << user->get_login() << ", room=" << (*it_room)->get_name() << ")";
									});
								}
								break;
							}
						}
					}
				}
				else {
					for (std::list<std::unique_ptr<Room>>::iterator it_room = m_server->m_rooms.begin(); it_room != m_server->m_rooms.end(); it_room++) {
						if ((*it_room)->get_name() == packet.get_arguments()[0]) {
							// Check room owner
							if ((*it_room)->get_owner() == user->get_login()) {
								for (unsigned int i = 1; i < packet.get_arguments_count(); i++) {
									for (std::list<User*>::iterator it_user = (*it_room)->get_users_list().begin(); it_user != (*it_room)->get_users_list().end(); it_user++) {
										if ((*it_user)->get_login() == packet.get_arguments()[i]) {
											(*it_room)->erase_user((*it_user));
											send(user, CommandPacket(CommandPacket::ChangeRoom, std::vector<std::wstring>()));
											Truelog::sync_print([&]() {
												Truelog::stream() << Truelog::Type::Info << "User kicked from his room (login=" << user->get_login() << ", room=" << (*it_room)->get_name() << ")";
											});
											break;
										}
									}
								}
							}
							break;
						}
					}
				}
				// Broadcast all user about changes
				for (std::list<std::unique_ptr<User>>::iterator it_user = m_server->m_users.begin(); it_user != m_server->m_users.end(); it_user++) {
					m_events[CommandPacket::RoomList](it_user->get(), CommandPacket(CommandPacket::RoomList, std::vector<std::wstring>()));
				}
			}
		},
		
		/*
		** ==== Room settings packet ====
		** 
		** If argument count is one then send the room settings to user:
		** -- REQUEST --
		** {
		**     room_name
		** }
		** -- RESPONSE --
		** {
		**     room_old_name
		**     room_new_name,
		**     room_owner,
		**     white_player,
		**     black_player,
		**     room_privacy,
		**     room_chat_enabled
		** }
		** 
		** Else, if argument count is seven and user is owner of this room then update
		**   the room settings and send to all users who are in this room:
		**
		** -- REQUEST --
		** {
		**     room_old_name
		**     room_new_name,
		**     room_owner,
		**     white_player,
		**     black_player,
		**     room_privacy,
		**     room_chat_enabled
		** }
		** -- RESPONSE --
		** {
		**     room_old_name
		**     room_new_name,
		**     room_owner,
		**     white_player,
		**     black_player,
		**     room_privacy,
		**     room_chat_enabled
		** }
		** 
		** If second argument is empty and user is owner the room then erase this room and
		**   kick all room member
		**/
		{
			CommandPacket::RoomSettings,
			[this](User* user, CommandPacket packet) {
				// If argument count is one then send to user information about the room
				if (packet.get_arguments_count() == 1) {
					for (std::list<std::unique_ptr<Room>>::iterator it_room = m_server->m_rooms.begin(); it_room != m_server->m_rooms.end(); it_room++) {
						if ((*it_room)->get_name() == packet.get_arguments()[0]) {
							CommandPacket settings_packet(CommandPacket::RoomSettings, std::vector<std::wstring>
							({
								(*it_room)->get_name(),
								(*it_room)->get_name(),
								(*it_room)->get_owner(),
								(*it_room)->get_white_player(),
								(*it_room)->get_black_player(),
								std::to_wstring(static_cast<int>((*it_room)->is_private())),
								std::to_wstring(static_cast<int>((*it_room)->is_chat_enabled()))
							}));

							Truelog::sync_print([&]() {
								std::wstring print_packet =
									L"\n    room_old_name     = " + (*it_room)->get_name() +
									L"\n    room_new_name     = " + (*it_room)->get_name() +
									L"\n    room_owner        = " + (*it_room)->get_owner() +
									L"\n    white_player      = " + (*it_room)->get_white_player() +
									L"\n    black_player      = " + (*it_room)->get_black_player() +
									L"\n    room_privacy      = " + std::to_wstring(static_cast<int>((*it_room)->is_private())) +
									L"\n    room_chat_enabled = " + std::to_wstring(static_cast<int>((*it_room)->is_chat_enabled()));
								Truelog::stream() << Truelog::Type::Info << "Room settings prepared to user (login=" << user->get_login() << ")"
									<< "\n{"
									<< print_packet
									<< "\n}";
							});
							send(user, settings_packet);
							
							break;
						}
					}
				}
				else if (packet.get_arguments_count() == 7) {
					// Min room name is 4 and max - 20
					if (packet.get_arguments()[1].size() > 0 && (packet.get_arguments()[1].size() < 4 || packet.get_arguments()[1].size() > 20)) {
						return;
					}
					Room* room = nullptr;
					for (std::list<std::unique_ptr<Room>>::iterator it_room = m_server->m_rooms.begin(); it_room != m_server->m_rooms.end(); it_room++) {
						if ((*it_room)->get_name() == packet.get_arguments()[0]) {
							// If room owner is the user then change the room otherwise ignore room editing
							if ((*it_room)->get_owner() == user->get_login()) {
								room = it_room->get();
								break;
							}
							else {
								return;
							}
						}
					}

					// If room's new name is empty and the room is exist then erase it
					if (packet.get_arguments()[1].size() == 0 && room != nullptr) {
						std::wstring erased_room_name = room->get_name();

						for (std::list<User*>::iterator it_user = room->get_users_list().begin(); it_user != room->get_users_list().end(); it_user++) {
							m_events[CommandPacket::ChangeRoom]((*it_user), CommandPacket(CommandPacket::ChangeRoom, std::vector<std::wstring>()));
						}
						m_server->m_room_manager.erase_room(room);

						Truelog::sync_print([&]() {
							Truelog::stream() << Truelog::Type::Info << "Room was erased (room_name=" << erased_room_name << ")"; 
						});
					}
					else {
						Room* room_ptr = m_server->m_room_manager.change_room_settings(					
							packet.get_arguments()[0],
							packet.get_arguments()[1],
							packet.get_arguments()[2],
							packet.get_arguments()[3],
							packet.get_arguments()[4],
							static_cast<bool>(std::stoi(packet.get_arguments()[5])),
							static_cast<bool>(std::stoi(packet.get_arguments()[6]))
						);

						Truelog::sync_print([&]() {
							std::wstring print_packet =
								L"\n    room_old_name     = " + packet.get_arguments()[0] +
								L"\n    room_new_name     = " + packet.get_arguments()[1] +
								L"\n    room_owner        = " + packet.get_arguments()[2] +
								L"\n    white_player      = " + packet.get_arguments()[3] +
								L"\n    black_player      = " + packet.get_arguments()[4] +
								L"\n    room_privacy      = " + packet.get_arguments()[5] +
								L"\n    room_chat_enabled = " + packet.get_arguments()[6];
							Truelog::stream() << Truelog::Type::Info << "Room settings changed (login=" << user->get_login() << ")" 
								<< "\n{"
								<< print_packet
								<< "\n}";
						});
						if (user->get_room() != nullptr) {
							room_ptr->add_user(user);
						}

						// Send new changes to users
						CommandPacket settings_packet(CommandPacket::RoomSettings, std::vector<std::wstring>(
						{
							packet.get_arguments()[0],
							room_ptr->get_name(),
							room_ptr->get_owner(),
							room_ptr->get_white_player(),
							room_ptr->get_black_player(),
							std::to_wstring(static_cast<int>(room_ptr->is_private())),
							std::to_wstring(static_cast<int>(room_ptr->is_chat_enabled()))
						}));
						for (std::list<User*>::iterator it_user = room_ptr->get_users_list().begin(); it_user != room_ptr->get_users_list().end(); it_user++) {
							send(*it_user, settings_packet);
						}
					}
					// Broadcast all user about changes
					for (std::list<std::unique_ptr<User>>::iterator it_user = m_server->m_users.begin(); it_user != m_server->m_users.end(); it_user++) {
						m_events[CommandPacket::RoomList](it_user->get(), CommandPacket(CommandPacket::RoomList, std::vector<std::wstring>()));
					}
				}
			}
		},

		/*
		** ==== Change role packet ====
		**
		** enum class Role {
		**     None = -1,
		**     Spectator = 0,
		**     White_player = 1,
		**     Black_player = 2
		** };
		**
		** -- REQUEST --
		** {
		**     new_role = { 0, 1, 2}
		** }
		**
		** -- RESPONSE --
		** {
		**     new_role
		** }
		**/
		{
			CommandPacket::ChangeRole,
			[this](User* user, CommandPacket packet) {
				if (packet.get_arguments_count() == 1) {
					if (user->get_room() != nullptr) {
						User::Role new_role;
					}
				}
			}
		},

		/*
		** ==== Figure description ====
		** 
		** 2 argument - move figure
		** 3 arguments - pawn has reached to the other end and change ne figure
		** 4 arguments - castling
		**
		** -- REQUEST --
		** {
		**     room_name,
		**     [old_position, new_position],
		**     [....],
		** }
		** -- RESPONSE --
		** {
		**     room_name,
		**     [old_position, new_position],
		**     [....],
		** }
		**
		** Pawn changes:
		** -- REQUEST --
		** {
		**     room_name,
		**     [old_position, new_position],
		**     new_type_figure
		** }
		** -- RESPONSE --
		** {
		**     room_name,
		**     [old_position, new_position],
		**     new_type_figure
		** }
		**
		** Castling:
		** -- REQUEST --
		** {
		**     room_name,
		**     [king_old_position, king_new_position],
		**     [rook_old_position, rook_position],
		**     empty
		** }
		** -- RESPONSE --
		** {
		**     room_name,
		**     [king_old_position, king_new_position],
		**     [rook_old_position, rook_position],
		**     empty
		** }
		**/
		{
			CommandPacket::FigureMove,
			[this](User* user, CommandPacket packet) {
				if (packet.get_arguments_count() >= 2) {
					Room* current_room = nullptr;
					for (std::list<std::unique_ptr<Room>>::iterator it_room = m_server->m_rooms.begin(); it_room != m_server->m_rooms.end(); it_room++) {
						if ((*it_room)->get_name() == packet.get_arguments()[0]) {
							current_room = it_room->get();
							break;
						}
					}

					// Checking for the existence of the room
					if (current_room == nullptr) {
						return;
					}

					// Checking for the existence player
					Figure::Color correct_figure_color;

					if (current_room->get_white_player() == user->get_login()) {
						correct_figure_color = Figure::Color::White;
					}
					else if (current_room->get_black_player() == user->get_login()) {
						correct_figure_color = Figure::Color::Black;
					}
					else {
						return;
					}



					if (packet.get_arguments()[1].size() == 2) {
						Figure* current_figure = current_room->get_chess_board()->get_figure(packet.get_arguments()[1].at(0));
						if (current_figure != nullptr) {
							Figure::Color figure_color = current_figure->get_color();
							if (figure_color != correct_figure_color) {
								return;
							}
						}
						else {
							return;
						}
					}
					else {
						return;
					}

					// Checking correct moves order
					if (correct_figure_color == Figure::Color::White) {
						if (current_room->get_chess_board()->get_turn_number() % 2 != 0) {
							return;
						}
					}
					else {
						if (current_room->get_chess_board()->get_turn_number() % 2 != 1) {
							return;
						}
					}

					bool has_moved = false;
					// Moving figure
					if (packet.get_arguments_count() == 2) {
						has_moved = current_room->get_chess_board()->move(packet.get_arguments()[1]);
					}
					// Changing pawn
					else if (packet.get_arguments_count() == 3) {
						has_moved = current_room->get_chess_board()->move(packet.get_arguments()[1]);

						if (has_moved) {
							has_moved = current_room->get_chess_board()->changing_pawn(packet.get_arguments()[1], packet.get_arguments()[2]);
						}
					}
					// Castling
					else if (packet.get_arguments_count() == 4) {
						has_moved = current_room->get_chess_board()->castling(packet.get_arguments()[1], packet.get_arguments()[2]);
					}

					if (has_moved) {
						//CommandPacket sending_packet(CommandPacket::FigureMove, packet);
						for (std::list<User*>::iterator it_user = current_room->get_users_list().begin(); it_user != current_room->get_users_list().end(); it_user++) {
							send((*it_user), packet);
						}
					}
				}
			}
		},

		{
			CommandPacket::BoardState,
			[this](User* user, CommandPacket packet) {
				if (packet.get_arguments_count() == 1) {
					Room* current_room = nullptr;
					for (std::list<std::unique_ptr<Room>>::iterator it_room = m_server->m_rooms.begin(); it_room != m_server->m_rooms.end(); it_room++) {
						if ((*it_room)->get_name() == packet.get_arguments()[0]) {
							current_room = it_room->get();
						}
					}
					
					if (current_room == nullptr) {
						return;
					}

					send(user, CommandPacket(CommandPacket::BoardState, current_room->get_chess_board()->get_board_state()));
				}
			}
		},

		/*
		** ==== Message packet ====
		** {
		**     room_name,
		**     login,
		**     message
		** }
		**/
		{
			CommandPacket::SendMessage,
			[this](User* user, CommandPacket packet) {
				if (packet.get_arguments_count() == 3) {
					Truelog::sync_print([&]() {
						Truelog::stream() << Truelog::Type::Info << "User sent message"
							<< "\n{"
							<< "\n    room    =" << packet.get_arguments()[0]
							<< "\n    login   =" << packet.get_arguments()[1]
							<< "\n    message =" << packet.get_arguments()[2]
							<< "\n}";
					});
					for (std::list<std::unique_ptr<Room>>::iterator it_room = m_server->m_rooms.begin(); it_room != m_server->m_rooms.end(); it_room++) {
						if ((*it_room)->get_name() == packet.get_arguments()[0]) {
							for (std::list<User*>::iterator it_user = (*it_room)->get_users_list().begin(); it_user != (*it_room)->get_users_list().end(); it_user++) {
								(*it_user)->get_socket()->send(packet.to_sfml_packet());
								send((*it_user), CommandPacket(CommandPacket::SendMessage, std::vector<std::wstring>
								({
									packet.get_arguments()[1],
									packet.get_arguments()[2]
								})));
							}
							break;
						}
					}
				}
			}
		}
	};
}

void PacketHandler::start()
{
	m_packets_handler_thread = std::thread(&PacketHandler::handler, this);

	Truelog::stream() << Truelog::Type::Info << "Start PacketHandler";

	m_server->m_socket_selector.add(m_server->m_listener);
}

void PacketHandler::stop()
{
	Truelog::stream() << Truelog::Type::Info << "Stop PacketHandler";

	m_server->m_socket_selector.clear();
}

void PacketHandler::send(User* user, CommandPacket packet)
{
	sf::Socket::Status status = user->get_socket()->send(packet.to_sfml_packet());

	Truelog::sync_print([&]() {
		std::wstring print_packet = L"";
		for (size_t i = 0; i < packet.get_arguments_count(); i++) {
			print_packet += (L"\n    arg[" + std::to_wstring(i) + L"]      = " + packet.get_arguments()[i]);
		}
		Truelog::stream() << Truelog::Type::Info << "Packet send to user (status=" << status << ", size=" << packet.to_sfml_packet().getDataSize() << "):"
			<< "\n{"
			<< "\n    packet_type = " << CommandPacket::packet_types[packet.get_command()]
			<< "\n    args_count  = " << packet.get_arguments_count()
			<< print_packet
			<< "\n}";
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
						Truelog::stream() << Truelog::Type::Info << "User connected to server (ip=" << userSocket->getRemoteAddress().toString() << ")";
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
							Truelog::stream() << Truelog::Type::Info << "Packet received (status=" << packet_status << ", size=" << sf_packet.getDataSize() << ")";
						});

						if (packet_status == sf::Socket::Done) {
							if (packet.is_valid()) {
								std::map<signed char, std::function<void(User*, CommandPacket)>>::iterator it_event = m_events.find(packet.get_command());

								if (it_event != m_events.end()) {
									std::thread([&](User* user, CommandPacket packet) {
										Truelog::sync_print([&]() {
											std::wstring print_packet = L"";
											for (size_t i = 0; i < packet.get_arguments_count(); i++) {
												print_packet += (L"\n    arg[" + std::to_wstring(i) + L"]      = " + packet.get_arguments()[i]);
											}
											Truelog::stream() << Truelog::Type::Info << "User sent packet"
												<< "\n{"
												<< "\n    packet_type = " << CommandPacket::packet_types[packet.get_command()]
												<< "\n    args_count  = " << packet.get_arguments_count()
												<< print_packet
												<< "\n}";
										});

										it_event->second(user, packet);
									}, it_user->get(), packet).detach();
								}
							}

							it_user++;
						}
						else if (packet_status == sf::Socket::Disconnected) {

							Truelog::sync_print([&]() {
								Truelog::stream() << Truelog::Type::Debug << "Removing user from his room...";
							});

							// Delete user from his room
							m_events[CommandPacket::Signout](it_user->get(), CommandPacket(CommandPacket::Signout, std::vector<std::wstring>()));

							Truelog::sync_print([&]() {
								Truelog::stream() << Truelog::Type::Debug << "Remove socket from socket selector...";
							});

							m_server->m_socket_selector.remove(*(*it_user)->get_socket());

							Truelog::sync_print([&]() {
								Truelog::stream() << Truelog::Type::Debug << "Disconnect socket...";
							});

							(*it_user)->get_socket()->disconnect();

							Truelog::sync_print([&]() {
								Truelog::stream() << Truelog::Type::Debug << "Remove user from user list...";
							});

							Truelog::sync_print([&]() {
								Truelog::stream() << Truelog::Type::Info << "User disconnected from server (ip=" << (*it_user)->get_socket()->getRemoteAddress().toString() << ")";
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
