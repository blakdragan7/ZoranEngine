#include "stdafx.h"
#include "WindowsWindow.h"
#include <iostream>

static LRESULT CALLBACK wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

WindowsWindow::WindowsWindow(RenderEngine* engine) :  WindowBase(engine)
{

}

WindowsWindow::~WindowsWindow()
{
}

bool WindowsWindow::MakeWindow(const char* title,int x, int y, int w, int h)
{
	WNDCLASS  wc;

	// Dialog custom window class
	ZeroMemory(&wc, sizeof(wc));
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbWndExtra = DLGWINDOWEXTRA;
	wc.hCursor = NULL;
	wc.hIcon = NULL;
	wc.lpfnWndProc = wndProc;
	wc.lpszClassName = title;

	if (!RegisterClass(&wc))
	{
		std::cerr << "Error Registering class : " << GetLastError() << "\n";
		return false;
	}

	// Create main application window
	windowHandle = CreateWindow(title, title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, w, h, NULL, NULL, GetModuleHandle(NULL), NULL);

	if (!windowHandle)
	{
		std::cerr << "Error Creating Window Code: " << GetLastError() << "\n";
		return false;
	}

	hwnd = (HWND)windowHandle;

	ShowWindow(hwnd, 1);

	SetWindowLong(hwnd, GWLP_USERDATA, (LONG)this);

	position.x = x;
	position.y = y;
	size.x = w;
	size.y = h;

	dc = GetDC(hwnd);

	return true;
}

void WindowsWindow::SetWindowFullScreen(bool isFullScreen_)
{

	// Save current window state if not already fullscreen.
	if (!isFullScreen) {
		// Save current window information.  We force the window into restored mode
		// before going fullscreen because Windows doesn't seem to hide the
		// taskbar if the window is in the maximized state.
		
		
		SendMessage(hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
		windowStyle = GetWindowLong(hwnd, GWL_STYLE);
		windowEXStyle = GetWindowLong(hwnd, GWL_EXSTYLE);

		GetWindowRect(hwnd, &windowRect);
		SetSizeAndPositionByRect(windowRect);
	}

	isFullScreen = isFullScreen_;

	if (isFullScreen_) {
		// Set new window style and size.
		SetWindowLong(hwnd, GWL_STYLE,
			windowStyle & ~(WS_CAPTION | WS_THICKFRAME));
		SetWindowLong(hwnd, GWL_EXSTYLE,
			windowEXStyle & ~(WS_EX_DLGMODALFRAME |
				WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE));

		// On expand, if we're given a window_rect, grow to it, otherwise do
		// not resize.
		
		MONITORINFO monitor_info;
		monitor_info.cbSize = sizeof(monitor_info);
		GetMonitorInfo(MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST),
			&monitor_info);
		;
		SetWindowPos(hwnd, NULL, monitor_info.rcMonitor.left, monitor_info.rcMonitor.top,
			monitor_info.rcMonitor.right - monitor_info.rcMonitor.left, monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top,
			SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
		
	}
	else {
		// Reset original window style and size.  The multiple window size/moves
		// here are ugly, but if SetWindowPos() doesn't redraw, the taskbar won't be
		// repainted.  Better-looking methods welcome.
		SetWindowLong(hwnd, GWL_STYLE, windowStyle);
		SetWindowLong(hwnd, GWL_EXSTYLE, windowEXStyle);

		
		if (isMaximized) ::SendMessage(hwnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	}
}

void WindowsWindow::SetSizeAndPositionByRect(RECT rect)
{
	size.y = rect.bottom - rect.top;
	size.x = rect.right - rect.left;
	size.y = rect.top;
	size.x = rect.left;
}

void WindowsWindow::SetPosition(int x, int y)
{
	SetWindowPos(hwnd, NULL, x, y, size.x, size.y, SWP_NOZORDER | SWP_NOSIZE | SWP_SHOWWINDOW);
	position.x = x;
	position.y = y;
}

void WindowsWindow::SetSize(int x, int y)
{
	SetWindowPos(hwnd, NULL, position.x, position.y, x, y, SWP_NOZORDER | SWP_SHOWWINDOW | SWP_NOMOVE);
	size.x = x;
	size.y = y;
}

void WindowsWindow::MakeActive()
{
	SetActiveWindow(hwnd);
}

void WindowsWindow::SwapBuffers()
{
	::SwapBuffers(dc);
}

static LRESULT CALLBACK wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	if (uMsg == WM_NCCREATE)
	{
		return TRUE;
	}
	if (uMsg == WM_CREATE)
	{
		return TRUE;
	}
	if (uMsg == WM_QUIT)
	{
		exit(0);
	}
	if (uMsg == WM_CLOSE)
	{
		exit(0);
	}
	if (uMsg == WM_SIZE)
	{

	}
	if (uMsg == WM_KEYDOWN)
	{

	}
	if (uMsg == WM_LBUTTONDOWN)
	{

	}
	if (uMsg == WM_RBUTTONDOWN)
	{

	}
	if (uMsg == WM_LBUTTONUP)
	{

	}
	if (uMsg == WM_RBUTTONUP)
	{


	}
	if (uMsg == WM_MOUSEMOVE)
	{

	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}