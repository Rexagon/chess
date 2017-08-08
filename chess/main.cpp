#include "Core.h"

#include "MainMenu.h"

int main()
{
    setlocale(LC_ALL, "Russian");

	try {
		Core::init();
		Core::add_state<MainMenu>();
		Core::run();
	}
	catch (const std::exception& e) {
		Log::write("ERROR:", e.what());
	}

	Core::close();

	return 0;
}
