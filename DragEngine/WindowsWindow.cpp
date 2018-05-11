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

void WindowsWindow::MakeWindow(const char* title,int x, int y, int w, int h)
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
		return;
	}

	// Create main application window
	windowHandle = CreateWindow(title, title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, w, h, NULL, NULL, GetModuleHandle(NULL), NULL);


	if (!windowHandle)
	{
		std::cerr << "Error Creating Window Code: " << GetLastError() << "\n";
	}
	
	ShowWindow((HWND)windowHandle, 1);

	SetWindowLong((HWND)windowHandle, GWLP_USERDATA, (LONG)this);
}

void WindowsWindow::MakeFullScreen()
{

}

void WindowsWindow::SetPosition(int x, int y)
{

}

void WindowsWindow::SetSize(int x, int y)
{

}

void WindowsWindow::MakeActive()
{

}

void WindowsWindow::GetPosition()
{

}

void WindowsWindow::SwapBuffers()
{
	::SwapBuffers(GetDC((HWND)windowHandle));
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