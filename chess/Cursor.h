#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#ifdef _WIN32
#include <windows.h>
#elif defined(__linux__)
#include <X11/cursorfont.h>
#include <X11/Xlib.h>
#else
#error This OS is not supported by the cursor library.
#endif

class SystemCursor
{
public:
	enum Type { 
		Wait, 
		Text, 
		Normal, 
		Hand
	};

	SystemCursor(const Type type);
	~SystemCursor();

	void set(const sf::WindowHandle& aWindowHandle) const;
private:
#ifdef SFML_SYSTEM_WINDOWS
	HCURSOR m_cursor;
#else
	XID m_cursor;
	Display* m_display;
#endif
};