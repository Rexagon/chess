#include "Cursor.h"

#ifdef _WIN32

SystemCursor::SystemCursor(const SystemCursor::Type type)
{
	switch (type)
	{
	case SystemCursor::Wait:
		m_cursor = LoadCursor(NULL, IDC_WAIT);
		break;
	case SystemCursor::Hand:
		m_cursor = LoadCursor(NULL, IDC_HAND);
		break;
	case SystemCursor::Normal:
		m_cursor = LoadCursor(NULL, IDC_ARROW);
		break;
	case SystemCursor::Text:
		m_cursor = LoadCursor(NULL, IDC_IBEAM);
		break;
	}
}

SystemCursor::~SystemCursor()
{
}

void SystemCursor::set(const sf::WindowHandle& aWindowHandle) const
{
	SetClassLongPtr(aWindowHandle, GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(m_cursor));
}

#elif defined(__linux__)

StandardCursor::StandardCursor(const StandardCursor::Type type)
{
	m_display = XOpenDisplay(NULL);

	switch (type)
	{
	case StandardCursor::Wait:
		m_cursor = XCreateFontCursor(m_display, XC_watch);
		break;
	case StandardCursor::Hand:
		m_cursor = XCreateFontCursor(m_display, XC_hand1);
		break;
	case StandardCursor::Normal:
		m_cursor = XCreateFontCursor(m_display, XC_left_ptr);
		break;
	case StandardCursor::Text:
		m_cursor = XCreateFontCursor(m_display, XC_xterm);
		break;
	}
}

StandardCursor::~StandardCursor()
{
	XFreeCursor(m_display, m_cursor);
	delete m_display;
	m_display = nullptr;
}

void StandardCursor::set(const WindowHandle& aWindowHandle) const
{
	XDefineCursor(m_display, aWindowHandle, m_cursor);
	XFlush(m_display);
}

#else
#error This OS is not yet supported by the cursor library.
#endif