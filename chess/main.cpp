#include "Core.h"

int main()
{
    setlocale(LC_ALL, "Russian");

	try {
		Core::init();

		Core::run();
	}
	catch (const std::exception& e) {
		Log::write("ERROR:", e.what());
	}

	Core::close();

	return 0;
}
