#include <exception>

#include "Server.h"
#include "Truelog.h"

int main()
{
	Truelog::init("server_log.txt");

	try {
		Server server;
		server.run();
	}
	catch (std::exception& exc) {
		Truelog::stream(Truelog::StreamType::All) << Truelog::Type::Error << exc.what();
	}
	return 0;
}