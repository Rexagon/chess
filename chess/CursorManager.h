#pragma once

#include <vector>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#ifdef _WIN32
#include <windows.h>

#ifdef SendMessage
#undef SendMessage
#endif
#elif defined(__linux__)
#include <X11/cursorfont.h>
#include <X11/Xlib.h>
#endif

class CursorManager
{
public:
	enum Type { 
		Wait, 
		Text, 
		Normal, 
		Hand,

		TypesCount
	};

	static void init(const sf::WindowHandle& window_handle);
	static void clear();

	static void set_style(const Type type);
private:
	static sf::WindowHandle m_window_handle;

#ifdef SFML_SYSTEM_WINDOWS
	static std::vector<HCURSOR> m_cursor_icons;
#else
	static std::vector<XID> m_cursor_icons;
	static Display* m_display;
#endif
};