/*
Window.h

Author:			Chris Serson
Last Edited:	October 12, 2016

Description:	Class for creating a window in a Win32 application.

Usage:			Calling the constructor, either through Window WIN(...); 
				or Window* WIN; WIN = new Window(...);, will initialize
				and create a window.
				Proper shutdown is handled by the destructor.
Future Work:	- Add ability to switch between windowed and full screen modes
				at run time.
				- Add resize window option.
				- Add ability to change resolution when in full screen. Currently
				Defaults to current monitor resolution.
				- Make some of the hard coded options more generic for future use.
				ie, ShowCursor(false); called in constructor should be optional and toggleable.
*/
#pragma once

#include <Windows.h>
#include <stdexcept>

namespace window {
	class Window_Exception : public std::runtime_error {
	public:
		Window_Exception(const char *msg) : std::runtime_error(msg) {}
	};

	class Window
	{
	public:
		Window(LPCWSTR appName, int height, int width, WNDPROC WndProc, bool isFullscreen);
		~Window();

		HWND GetWindow() { return m_Window; }
		int Height() { return m_hWindow; }
		int Width() { return m_wWindow; }

	private:
		HINSTANCE m_Instance;
		HWND m_Window;
		bool m_isFullscreen;
		int m_hWindow;
		int m_wWindow;
		LPCWSTR m_nameApp;
	};
}