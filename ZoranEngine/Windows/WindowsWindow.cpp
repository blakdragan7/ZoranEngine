#include "stdafx.h"
#include "WindowsWindow.h"
#include <Rendering/RenderEngineBase.h>
#include <Core/ZoranEngine.h>
#include <iostream>

#include <Windows/WindowsMouse.h>

#include <ZGI/Core/ZGIVirtualWindow.h>

#include <ThirdParty/imgui/imgui.h>
#include <ThirdParty/imgui/imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static LRESULT CALLBACK wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

WindowsWindow::WindowsWindow(ZoranEngine* engine) :  WindowBase(engine)
{
	hwnd = 0;
	dc = 0;
	m = new WindowsMouse;
}

WindowsWindow::~WindowsWindow()
{
	if (hwnd)
	{
		ImGui_ImplWin32_Shutdown();

		CloseWindow(hwnd);
		DestroyWindow(hwnd);

		ImGui::DestroyContext();
	}
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
	windowHandle = CreateWindow(title, title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, w, h, NULL, NULL, GetModuleHandle(NULL), (LPVOID)this);

	if (!windowHandle)
	{
		std::cerr << "Error Creating Window Code: " << GetLastError() << "\n";
		return false;
	}

	position.x = x;
	position.y = y;
	size.w = w;
	size.h = h;

	hwnd = (HWND)windowHandle;

	ShowWindow(hwnd, 1);

	dc = GetDC(hwnd);
	
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplWin32_Init(hwnd);

	return true;
}

void WindowsWindow::SetWindowFullScreen(bool isFullScreen_)
{

	// Save current window state if not already fullscreen.
	if (!isFullScreen) {
		// Save current window information.  We force the window longo restored mode
		// before going fullscreen because Windows doesn't seem to hide the
		// taskbar if the window is in the maximized state.
		SendMessage(hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
		windowStyle = GetWindowLong(hwnd, GWL_STYLE);
		windowEXStyle = GetWindowLong(hwnd, GWL_EXSTYLE);

		GetWindowRect(hwnd, &windowRect);
	}

	if (isFullScreen_) {
		isFullScreen = isFullScreen_;

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
		SetWindowLong(hwnd, GWL_STYLE, windowStyle);
		SetWindowLong(hwnd, GWL_EXSTYLE, windowEXStyle);

		if (isMaximized) ::SendMessage(hwnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		else
		{
			SetSizeAndPositionByRect(windowRect);
		}

		isFullScreen = isFullScreen_;
	}
}

void WindowsWindow::SetSizeAndPositionByRect(RECT rect)
{
	size.h = rect.bottom - rect.top;
	size.w = rect.right - rect.left;
	position.y = rect.top;
	position.x = rect.left;

	SetWindowPos(hwnd, NULL, position.x, position.y, size.x, size.y, SWP_NOZORDER | SWP_SHOWWINDOW);
}

unsigned WindowsWindow::ConvertWPARAMToKey(WPARAM key)
{
	if(key < 0x70 || key > 0x87)return static_cast<unsigned>(key); // ascii
	return static_cast<unsigned>(key + 0x378);
}

void WindowsWindow::SetPosition(long x, long y)
{
	SetWindowPos(hwnd, NULL, (int)x, (int)y, (int)size.x, (int)size.y, SWP_NOZORDER | SWP_NOSIZE | SWP_SHOWWINDOW);
	position.x = x;
	position.y = y;
}

void WindowsWindow::SetSize(long x, long y)
{
	SetWindowPos(hwnd, NULL, (int)position.x, (int)position.y, (int)x, (int)y, SWP_NOZORDER | SWP_SHOWWINDOW | SWP_NOMOVE);
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

void WindowsWindow::MainDraw()
{
	ImGui_ImplWin32_NewFrame();

	WindowBase::MainDraw();
}

static LRESULT CALLBACK wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
	
	WindowsWindow *pThis = 0;

	if (uMsg == WM_NCCREATE)
	{
		pThis = static_cast<WindowsWindow*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);

		SetLastError(0);
		if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis)))
		{
			if (GetLastError() != 0)
				return FALSE;
		}
	}
	else
	{
		pThis = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}

	if (pThis)
	{

		if (uMsg == WM_CREATE)
		{
			return TRUE;
		}
		if (uMsg == WM_QUIT)
		{
			zEngine->StopRunning();
		}
		if (uMsg == WM_CLOSE)
		{
			zEngine->StopRunning();
		}
		if (uMsg == WM_SIZE)
		{

			int width = LOWORD(lParam);
			int height = HIWORD(lParam);

			pThis->Resize({ width, height });
			
		}
		if (uMsg == WM_MOVE)
		{

			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			pThis->SetWindowPosNoExecute(x, y);
			
		}
		if (uMsg == WM_KEYDOWN)
		{
			zEngine->KeyEvent(KEY_DOWN, pThis->ConvertWPARAMToKey(wParam));
			pThis->rootVirtualWindow->KeyEvent(KEY_DOWN, pThis->ConvertWPARAMToKey(wParam));
		}
		if (uMsg == WM_KEYUP)
		{
			zEngine->KeyEvent(KEY_UP, pThis->ConvertWPARAMToKey(wParam));
			pThis->rootVirtualWindow->KeyEvent(KEY_UP, pThis->ConvertWPARAMToKey(wParam));
		}
		if (uMsg == WM_LBUTTONDOWN)
		{
			zEngine->MouseEvent(MOUSE_L_DOWN, 0);
			pThis->m->SetLeftMouseIsPressed(true);
			pThis->rootVirtualWindow->MouseDown(pThis->m);
		}
		if (uMsg == WM_RBUTTONDOWN)
		{
			zEngine->MouseEvent(MOUSE_R_DOWN, 0);
			pThis->m->SetRightMouseIsPressed(true);
			pThis->rootVirtualWindow->MouseDown(pThis->m);
		}
		if (uMsg == WM_LBUTTONUP)
		{
			zEngine->MouseEvent(MOUSE_L_UP, 0);
			pThis->m->SetLeftMouseIsPressed(false);
			pThis->rootVirtualWindow->MouseUp(pThis->m);
		}
		if (uMsg == WM_RBUTTONUP)
		{
			zEngine->MouseEvent(MOUSE_R_UP, 0);
			pThis->m->SetRightMouseIsPressed(false);
			pThis->rootVirtualWindow->MouseUp(pThis->m);
		}
		if (uMsg == WM_MOUSEMOVE)
		{
			float x = static_cast<float>(LOWORD(lParam));
			float y = static_cast<float>(HIWORD(lParam));

			zEngine->MouseMove(LOWORD(lParam),HIWORD(lParam));
			pThis->m->SetPosition({ x, y});
			pThis->rootVirtualWindow->MouseMove(pThis->m);
		}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}