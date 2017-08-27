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

std::fstream & operator<<(std::fstream & stream, const Figure & f)
{
	Figure::Description des = f.get_description();
	Figure::Position pos = f.get_pos();

	stream.write(reinterpret_cast<const char*>(&des), sizeof(des));
	stream.write(reinterpret_cast<const char*>(&pos), sizeof(pos));

	return stream;
}

std::fstream & operator >> (std::fstream & stream, Figure & f)
{
	Figure::Description des;
	Figure::Position pos;

	stream.read(reinterpret_cast<char*>(&des), sizeof(des));
	stream.read(reinterpret_cast<char*>(&pos), sizeof(pos));

	f.set_description(des);
	f.set_pos(pos);

	return stream;
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
	strftime(buf, sizeof(buf), "[%d.%m.%Y %X]", &tstruct);

	return buf;
}

std::wstring Stuff::time_to_wstring(time_t time)
{
	return std::to_wstring(time_to_string(time));
}
