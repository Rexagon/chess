#include "Log.h"

std::ofstream Log::m_file;
Log::Targets Log::m_target;

void Log::create(const std::string & path, Targets target)
{
	m_target = target;
	if (m_target == Targets::FILE || m_target == Targets::ALL) {
		m_file.open(path);
	}
}

const std::string Log::getDate()
{
	time_t now = time(0);
	struct tm* tstruct;
	char buf[80];
	tstruct = localtime(&now);
	strftime(buf, sizeof(buf), "[%d.%m.%Y %X] ", tstruct);

	return buf;
}

std::wostream & operator<<(std::wostream & stream, const std::string & string)
{
	stream << string.c_str();
	return stream;
}