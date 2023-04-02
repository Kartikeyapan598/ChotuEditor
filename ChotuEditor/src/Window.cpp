#include "Chotupch.h"

#include "Window.h"
#include "KeyCode.h"
#include "TextRender.h"

#include <tchar.h>
#include <dwmapi.h>
#include <commctrl.h>
#include <VersionHelpers.h>

#pragma comment(lib,"dwmapi.lib")
#pragma comment(lib,"Comdlg32.lib")
#pragma comment(lib, "comctl32.lib")

namespace CE
{
	void Window::ClearResources()
	{
	}

	Window* Window::GetInstance()
	{
		if (m_Instance == nullptr)
		{
			return new Window();
		}
		return m_Instance;
	}

	Window::Window()
	{
		m_Instance = this;
		Initialize();
	}

	Window::~Window()
	{
		delete m_textRenderer;
		m_textRenderer = NULL;
	}

	void OpenFileWindow(HWND hwnd)
	{
		wchar_t fileDir[100];

		OPENFILENAMEW ofn;
		ZeroMemory(&ofn, sizeof(OPENFILENAMEW));

		ofn.lStructSize = sizeof(OPENFILENAMEW);
		ofn.hwndOwner = hwnd;
		ofn.lpstrFile = fileDir;
		ofn.lpstrFile = NULL;
		ofn.nMaxFile = 100;
		ofn.lpstrFilter = _T("All Files\0");
		ofn.nFilterIndex = 1;

		GetOpenFileNameW(&ofn);
	}

	void Window::HandleDrawText(RECT &rc)
	{
		D2D1_RECT_F layoutRect;
		layoutRect.left = rc.left;
		layoutRect.right = rc.right;
		layoutRect.top = rc.top;
		layoutRect.bottom = rc.bottom;

		const WCHAR* ch = CharToWchar(m_buffer.data);

		DrawTextInfo drawInfo(D2D1::ColorF::Black, layoutRect, ch);
		m_textRenderer->HandleRenderText(drawInfo);
		delete[] ch;
	}

	void Window::HandleDrawCarat()
	{
		m_textRenderer->HandleRenderCarat();
	}

	LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		RECT rc;
		GetClientRect(hwnd, &rc);
		D2D1_SIZE_U sz = { (UINT32)(rc.right - rc.left), (UINT32)(rc.bottom - rc.top) };

		HRESULT hr;
		switch (msg)
		{
		case WM_CREATE:
		{
			Window* window = Window::GetInstance();
			window->m_textRenderer = new Renderer(hwnd, 10.0f, 30.0f);
			window->m_textRenderer->SetTextColor(D2D1::ColorF::White);
		}
			break;
		case WM_LBUTTONDOWN:
			break;
		case WM_SIZE :
		{
			Window* m_Window = Window::GetInstance();
			m_Window->ResizeRenderer(LOWORD(lparam), HIWORD(lparam));
			break;
		}
		case WM_DISPLAYCHANGE:
		case WM_PAINT :
		{
			Window* m_Window = Window::GetInstance();
			m_Window->HandleDrawText(rc);
			//m_Window->HandleDrawCarat();
			break;
		}
		case WM_QUIT:
		case WM_DESTROY :
		case VK_F4 :
		{
			DestroyWindow(hwnd);
			PostQuitMessage(0);
			Window::m_Running = false;
			break;
		}
		case WM_UNICHAR:
		case WM_CHAR:
		{
			Window* window = Window::GetInstance();
			if (window->unsaved == true)
			{
				window->unsaved = false;
				std::string title = window->m_data.m_title;
				title += '*';

				window->SetTitle(title);
			}

			char cchar = (static_cast<KeyCode>(wparam));
			if (cchar != CE::Key::Enter && cchar != CE::Key::Backspace)
			{
				std::cout << "Key Pressed! " << cchar << "\n";
				Window::Pos_count = CE::InsertCharacter(&Window::m_buffer, Window::Pos_count, cchar);
				//data->EventCall(event);
			}
			else if (cchar == CE::Key::Enter)
			{
				std::cout << "Enter Pressed!\n";
				Window::Pos_count = CE::InsertCharacter(&Window::m_buffer, Window::Pos_count, '\n');
			}
			else if (cchar == CE::Key::Backspace)
			{
				std::cout << "BackSpace Pressed!\n";
				Window::Pos_count = RemoveCharacter(&Window::m_buffer, Window::Pos_count);
				// Reduce m_buffer->end_pos by 1 or however times backspace is pressed!
			}
			else if (cchar == CE::Key::Tab)
			{
				std::cout << "Tab Pressed\n";
				Window::Pos_count = RemoveCharacter(&Window::m_buffer, Window::Pos_count);
			}

			if (cchar == CE::Key::LeftControl)
			{
				std::cout << "Create Change Prompt here\n";
			}
			break;
		}
		default :
			return DefWindowProc(hwnd, msg, wparam, lparam);
		}
		return 0;
	}

	void Window::SetTitle(const std::string& title)
	{
		m_data.m_title = title;

#if defined UNICODE
		std::wstring wTitle = std::wstring(m_data.m_title.begin(), m_data.m_title.end());
		LPCWSTR Title = wTitle.c_str();
		SetWindowText(m_hwnd, wTitle.c_str());
#elif defined ANSI
		SetWindowText(m_hwnd, _T(title.c_str()));
#endif
	}

	bool Window::Initialize()
	{
		std::cout << "Window Init Function Started! " << "\n";
		m_Running = true;
		

		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(wc));

		wc.cbClsExtra = NULL;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.cbWndExtra = NULL;
		wc.hbrBackground = CreateSolidBrush(RGB(55, 55, 55));	//(HBRUSH)GetStockObject(DKGRAY_BRUSH);;  COLOR_ACTIVEBORDER
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hIcon = LoadIcon(NULL, IDI_EXCLAMATION);				//(HICON)LoadImageW(nullptr, L"res\\paddle.ico", IMAGE_ICON, 64, 64, LR_LOADFROMFILE);
		wc.hIconSm = LoadIcon(NULL, IDI_EXCLAMATION);
		wc.hInstance = NULL;
		wc.lpszClassName = _T("Cndr");
		wc.lpszMenuName = _T("");
		wc.style = NULL;
		wc.lpfnWndProc = &WindowProc;

		if (!::RegisterClassEx(&wc))
		{
			return false;
		}

#ifdef UNICODE
		std::wstring wTitle = std::wstring(m_data.m_title.begin(), m_data.m_title.end());
		LPCWSTR Title = wTitle.c_str();
#else
		auto Title = m_data.m_title;
#endif 
		m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, wc.lpszClassName, Title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, m_data.m_width, m_data.m_height,
			NULL, NULL, NULL, NULL);

		if (!m_hwnd)
		{
			return false;
		}

		uint32_t setTitleBarDark = true;
		DwmSetWindowAttribute(m_hwnd, DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
			&setTitleBarDark, sizeof(setTitleBarDark));

		ShowWindow(m_hwnd, SW_SHOW);
		UpdateWindow(m_hwnd);

		return true;
	}

	void Window::broadcast()
	{
		MSG msg;
		std::cout << "Flag in Broadcast" << "\n";
		while (GetMessage(&msg, 0, 0, 0) != 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}