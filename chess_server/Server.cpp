#include "Server.h"

using namespace std::chrono_literals;

Server::Server(unsigned short port) : m_is_run(true), m_port(port), m_packet_handler(this), m_room_manager(this)
{
}

void Server::run()
{
	setlocale(LC_ALL, "");

	// If server not open with the port throw exception
	if (m_listener.listen(m_port) != sf::Socket::Done) {
		throw std::exception(std::string("Unable to create server on port " + std::to_string(m_port)).c_str());
	}

	// If db is open then create table if not exists, else throw exception
	if (sqlite3_open("./data/chess.db", &m_db) == SQLITE_OK) {
		std::ifstream sql("./data/structure.sql");
		if (!sql.is_open()) {
			throw std::exception("Can\'t open structure sql file.");
		}

		std::string sql_query((std::istreambuf_iterator<char>(sql)), std::istreambuf_iterator<char>());

		sqlite3_exec(m_db, sql_query.c_str(), 0, 0, NULL);
	}
	else {
		throw std::exception("Can\'t open or create a database.");
	}

	m_room_manager.init();

	Truelog::stream() << Truelog::Type::Info << "A database has opened";

	m_packet_handler.start();

	// Main server loop
	std::chrono::time_point<std::chrono::system_clock> lastTime = std::chrono::system_clock::now();
	while (m_is_run) {
		std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();

		// TODO: add server commands

		lastTime = currentTime;
		std::this_thread::sleep_until(currentTime + 8ms);
	}

	m_packet_handler.stop();
}
