#include "Truelog.h"

#include <time.h>

std::wofstream Truelog::m_logFile;
Truelog::StreamType Truelog::m_streamType;
unsigned int Truelog::m_warningCount;
unsigned int Truelog::m_errorCount;
std::mutex Truelog::m_logMutex;

void Truelog::init(const std::string & path)
{
	m_logFile.open(path);
	if (!m_logFile.is_open()) {
		throw std::exception(std::string("Truelog can\'t open file: \"" + path + "\"").c_str());
	}
	// App locale to stream
	m_logFile.imbue(std::locale(""));

	m_logFile
		<< "**********************\n"
		<< "*   Chess v0.0.1     *\n"
		<< "*   by Mechanic.     *\n" 
		<< "**********************\n";
	std::wcout
		<< "**********************\n"
		<< "*   Chess v0.0.1     *\n"
		<< "*   by Mechanic.     *\n"
		<< "**********************\n";
}

void Truelog::sync_print(std::function<void()> func)
{
	m_logMutex.lock();
	func();
	m_logMutex.unlock();
}

Truelog & Truelog::stream(Truelog::StreamType streamt)
{
	static Truelog log(streamt);
	log.m_streamType = streamt;
	return log;
}

const std::string Truelog::GetDate()
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "[%d.%m.%Y %X]", &tstruct);

	return buf;
}

std::wostream & operator<<(std::wostream& stream, const std::string& s)
{
	stream << s.c_str();
	return stream;
}

std::wstring std::to_wstring(const std::string & str)
{
	std::wstring result(str.size(), 0);

	std::copy(str.begin(), str.end(), result.begin());

	return result;
}
