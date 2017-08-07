#include "PacketHandler.h"

#include "Server.h"

PacketHandler::PacketHandler(Server * server) : m_is_run(false), m_server(server)
{
	m_packets_handler_thread = std::thread(&PacketHandler::Handler, this);
}

void PacketHandler::start()
{
	Truelog::stream(Truelog::StreamType::File) << Truelog::Type::Info << "Start PacketHandler";

	m_server->m_socket_selector.add(m_server->m_listener);

	m_is_run = true;
	m_condition_handling.notify_all();
}

void PacketHandler::stop()
{
	Truelog::stream(Truelog::StreamType::File) << Truelog::Type::Info << "Stop PacketHandler";

	m_server->m_socket_selector.clear();

	m_is_run = false;
	m_condition_handling.notify_all();
}

std::pair<User*, sf::Packet> * PacketHandler::pull_packet()
{
	std::unique_lock<std::mutex> lock(m_queue_mutex);
	if (m_packets_queue.empty()) {
		return nullptr;
	}
	else {
		m_packet_back = m_packets_queue.back();
		m_packets_queue.pop();
		return &m_packet_back;
	}
}

void PacketHandler::send(User* user, sf::Packet packet)
{
	m_status = user->get_socket()->send(packet);

	Truelog::sync_print([&]() {
		Truelog::stream(Truelog::StreamType::All) << Truelog::Type::Info << "Packet was sent to server (status=" << m_status << ")";
	});
}

const sf::Socket::Status PacketHandler::get_status() const
{
	return m_status;
}

bool PacketHandler::has_packets()
{
	if (m_packets_queue.empty()) {
		return false;
	}
	else {
		return true;
	}
}

void PacketHandler::Handler()
{
	std::unique_lock<std::mutex> handler_lock(m_handler_mutex);

	while (m_server->m_is_run) {
		m_condition_handling.wait(handler_lock, [this]() { return m_is_run; });
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
				for (std::list<std::unique_ptr<User>>::iterator it = m_server->m_users.begin(); it != m_server->m_users.end();) {
					if (m_server->m_socket_selector.isReady(*(*it)->get_socket())) {
						sf::Packet packet;
						m_status = (*it)->get_socket()->receive(packet);

						Truelog::sync_print([&]() {
							Truelog::stream(Truelog::StreamType::All) << Truelog::Type::Info << "Packet was received (status=" << m_status << ", size=" << packet.getDataSize() << ")";
						});

						if (m_status == sf::Socket::Done) {
							std::unique_lock<std::mutex> packet_lock(m_queue_mutex);
							m_packets_queue.push(std::pair<User*, sf::Packet>((*it).get(), packet));

							// Check events
							if (m_events.size() > 0) {
								sf::Packet tmp_packet = packet;
								sf::Int8 type;

								if (tmp_packet >> type) {
									std::unique_lock<std::mutex> event_lock(m_events_mutex);
									for (std::vector<std::pair<sf::Int8, std::function<void(sf::Packet*)>>>::iterator it = m_events.begin(); it != m_events.end(); it++)
									{
										if (type == it->first) {
											m_packet_back = m_packets_queue.back();
											m_packets_queue.pop();
											it->second(&m_packet_back.second);
											m_events.erase(it);
											break;
										}
									}
								}
							}
							it++;
						}
						else if (m_status == sf::Socket::Disconnected) {

							Truelog::sync_print([&]() {
								Truelog::stream(Truelog::StreamType::All) << Truelog::Type::Debug << "Removing user from his room...";
							});

							if ((*it)->get_room() != nullptr) {
								///(*it)->get_room()->HandleDisconnection(*(*it));
							}

							Truelog::sync_print([&]() {
								Truelog::stream(Truelog::StreamType::All) << Truelog::Type::Debug << "Remove socket from socket selector...";
							});

							m_server->m_socket_selector.remove(*(*it)->get_socket());

							Truelog::sync_print([&]() {
								Truelog::stream(Truelog::StreamType::All) << Truelog::Type::Debug << "Disconnect socket...";
							});

							(*it)->get_socket()->disconnect();

							Truelog::sync_print([&]() {
								Truelog::stream(Truelog::StreamType::All) << Truelog::Type::Debug << "Remove user from user list...";
							});

							Truelog::sync_print([&]() {
								Truelog::stream(Truelog::StreamType::All) << Truelog::Type::Info << "User was disconnected from server (ip=" << (*it)->get_socket()->getRemoteAddress().toString() << ")";
							});

							it = m_server->m_users.erase(it);
						}
					}
					else {
						it++;
					}
				}
			}
		}
	}
}

void PacketHandler::single_packet_event(sf::Int8 type, std::function<void(sf::Packet*)> callback)
{
	std::unique_lock<std::mutex> lock(m_events_mutex);
	m_events.push_back(std::pair<sf::Int8, std::function<void(sf::Packet*)>>(type, callback));
}
