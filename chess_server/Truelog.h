#pragma once

#include <functional>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <codecvt>
#include <string>
#include <mutex>

#include "Stuff.h"

class Truelog {
//private:
	class LogHelper;
public:
	enum class Type {
		Info,
		Error,
		Warning,
		Debug
	};

	enum class StreamType {
		Console,
		File,
		All
	};

	static void init(const std::string& path = "log.txt");

	static void sync_print(std::function<void()> func);

	// Write logs
	static Truelog& stream(Truelog::StreamType streamt = Truelog::StreamType::All);

	template <class Arg>
	LogHelper & operator<<(const Arg & arg)
	{
		if (m_streamType == Truelog::StreamType::All) {
			std::wcout << std::endl;
			m_logFile << std::endl;
		}
		else if (m_streamType == Truelog::StreamType::Console) {
			std::wcout << std::endl;
		}
		else {
			m_logFile << std::endl;
		}
		return (LogHelper(this) << arg);
	}

private:
	Truelog() = delete;
	Truelog(const Truelog&) = delete;
	Truelog(Truelog::StreamType stream) {}

	static std::wofstream m_logFile;

	static Truelog::StreamType m_streamType;

	static unsigned int m_warningCount;
	static unsigned int m_errorCount;

	static std::mutex m_logMutex;

	class LogHelper
	{
	public:
		LogHelper(Truelog* owner) : m_owner(owner) {}

		LogHelper& operator<<(const Truelog::Type& type)
		{
			std::wstring output;
			std::wstring tmp_date = Stuff::time_to_wstring(Stuff::get_current_time());
			switch (type)
			{
			case Truelog::Type::Info:
				output = L"[INFO] [" + tmp_date + L"]: ";
				break;
			case Truelog::Type::Error:
				output = L"[ERROR] [" + tmp_date + L"]: ";
				m_owner->m_errorCount++;
				break;
			case Truelog::Type::Warning:
				output = L"[WARNING] [" + tmp_date + L"]: ";
				m_owner->m_warningCount++;
				break;
			case Truelog::Type::Debug:
				output = L"[DEBUG] [" + tmp_date + L"]: ";
				break;
			default:
				break;
			}

			if (m_owner->m_streamType == Truelog::StreamType::All) {
				std::wcout << output << std::flush;
				m_logFile << output << std::flush;
			}
			else if (m_owner->m_streamType == Truelog::StreamType::Console) {
				std::wcout << output << std::flush;
			}
			else {
				m_logFile << output << std::flush;
			}
			return *this;
		}

		template<class Arg>
		LogHelper& operator<<(const Arg& arg)
		{
			if (m_owner->m_streamType == Truelog::StreamType::All) {
				std::wcout << arg << std::flush;
				m_logFile << arg << std::flush;
			}
			else if (m_owner->m_streamType == Truelog::StreamType::Console) {
				std::wcout << arg << std::flush;
			}
			else {
				m_logFile << arg << std::flush;
			}
			return *this;
		}

	private:
		Truelog* m_owner;
	};
};