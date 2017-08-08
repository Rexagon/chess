#include <exception>

#include <ConsoleConstructor.h>

#include "Server.h"
#include "Truelog.h"

using console = mech::ConsoleConstructor;

int main(int argc, char* argv[])
{
	console::setProgramName("Chess server v0.1");
	console::on("port", "-p", "--port", "Set server port. Default is 49876.");

	int res = console::consoleHandler(argc, argv);

	if (res == console::STATE::HELP) {
		return 0;
	}
	else if (res == console::STATE::WRONG_NUMBER_PARAMS) {
		return 1;
	}
	else if (res == console::STATE::WRONG_ARGUMENTS) {
		return 2;
	}

	unsigned short port = 49876;

	if (console::getStatusInstruction("port")) {
		port = atoi(console::getArguments("port")[0].c_str());
	}

	Truelog::init("server_log.txt");

	try {
		Server server(port);
		server.run();
	}
	catch (std::exception& exc) {
		Truelog::stream(Truelog::StreamType::All) << Truelog::Type::Error << exc.what();
	}
	return 0;
}