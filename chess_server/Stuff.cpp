#include "Stuff.h"

#include "Figure.h"

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

time_t Stuff::get_current_time()
{
	return time(0);
}

std::string Stuff::time_to_string(time_t time)
{
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&time);
	strftime(buf, sizeof(buf), "%d.%m.%Y %X", &tstruct);

	return buf;
}

std::wstring Stuff::time_to_wstring(time_t time)
{
	return std::to_wstring(time_to_string(time));
}
