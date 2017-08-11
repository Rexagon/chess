#include "CursorManager.h"

sf::WindowHandle CursorManager::m_window_handle = 0;

#ifdef _WIN32
std::vector<HCURSOR> CursorManager::m_cursor_icons(TypesCount);
#elif defined(__linux__)
std::vector<XID> CursorManager::m_cursor_icons(TypesCount);
Display* CursorManager::m_display = nullptr;
#endif

void CursorManager::init(const sf::WindowHandle & window_handle)
{
	m_window_handle = window_handle;

#ifdef _WIN32
	m_cursor_icons[Type::Wait] = LoadCursor(NULL, IDC_WAIT);
	m_cursor_icons[Type::Hand] = LoadCursor(NULL, IDC_HAND);
	m_cursor_icons[Type::Normal] = LoadCursor(NULL, IDC_ARROW);
	m_cursor_icons[Type::Text] = LoadCursor(NULL, IDC_IBEAM);
#elif defined(__linux__)
	m_display = XOpenDisplay(NULL);
	m_cursor_icons[Type::Wait] = XCreateFontCursor(m_display, XC_watch);
	m_cursor_icons[Type::Hand] = XCreateFontCursor(m_display, XC_hand1);
	m_cursor_icons[Type::Normal] = XCreateFontCursor(m_display, XC_left_ptr);
	m_cursor_icons[Type::Text] = XCreateFontCursor(m_display, XC_xterm);
#endif
}

void CursorManager::clear()
{
#ifdef __linux__
	for (unsigned int i = 0; i < TypesCount; ++i) {
		XFreeCursor(m_display, m_cursor_icons[i]);
	}
	delete m_display;
	m_display = nullptr;
#endif
}

void CursorManager::set_style(const Type type)
{
#ifdef _WIN32
	SetClassLongPtr(m_window_handle, GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(m_cursor_icons[type]));
#elif defined(__linux__)
	XDefineCursor(m_display, m_window_handle, m_cursor_icons[type]);
	XFlush(m_display);
#endif
}