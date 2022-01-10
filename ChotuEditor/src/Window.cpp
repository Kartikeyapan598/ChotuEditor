#include "Chotupch.h"
#include "Window.h"
#include "KeyCode.h"
#include "TextRender.h"

#include <tchar.h>
#include <commctrl.h>

#pragma comment(lib,"Comdlg32.lib")
#pragma comment(lib, "comctl32.lib")

namespace CE
{
	static HWND openMenu;
	LRESULT CALLBACK ChildProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	void Window::ClearResources()
	{
	}

	Window* Window::Create()
	{
		return new Window();
	}
	Window::Window()
	{
		if (!m_Instance)
		{
			m_Instance = this;
			HRESULT hr;

			hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_d2dfactory);
			hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&m_writeFactory);
			hr = m_writeFactory->CreateTextFormat(L"Arial", 0, DWRITE_FONT_WEIGHT_REGULAR,
				DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 36.0f, L"en-us", &m_text_format);
			//m_textlayout->HitTestTextPosition();
			Init();
		}
	}

	Window::~Window() 
	{
		RELEASE(m_writeFactory)
		RELEASE(m_d2dfactory)
		RELEASE(m_rendertarget)
		RELEASE(m_text_format)
		RELEASE(m_textBrush)
	}


	void DefMenu(HWND hwnd)
	{
		HMENU m_MainMenubar;
		HMENU m_fileMenu;
		HMENU m_AboutMenu;
		HMENU m_ViewMenu;
		HMENU m_TabMenu;

		m_MainMenubar = CreateMenu();
		m_fileMenu = CreateMenu();
		m_AboutMenu = CreateMenu();
		m_ViewMenu = CreateMenu();
		m_TabMenu = CreateMenu();

		AppendMenuW(m_fileMenu, MF_STRING, IDM_FILE_NEW, _T("&New"));
		AppendMenuW(m_fileMenu, MF_STRING, IDM_FILE_OPEN, _T("&Open"));
		AppendMenuW(m_fileMenu, MF_STRING, IDM_FILE_SAVE, _T("&Save		Ctrl+S"));
		AppendMenuW(m_fileMenu, MF_STRING, IDM_FILE_SAVE_AS, _T("&Save As"));
		AppendMenuW(m_fileMenu, MF_STRING, IDM_FILE_QUIT, _T("&Quit"));

		AppendMenuW(m_TabMenu, MF_STRING, IDM_FILE_TAB, _T("New Tab"));
		
		AppendMenuW(m_AboutMenu, MF_STRING, IDM_FILE_ABOUT, _T("&About"));
		AppendMenuW(m_AboutMenu, MF_STRING, IDM_APP_INFO, _T("&Application Info"));
		AppendMenuW(m_AboutMenu, MF_STRING, IDM_FILE_USAGE, _T("&Usage"));

		AppendMenuW(m_ViewMenu, MF_STRING, IDM_FILE_VIEW, _T("&View"));
		//AppendMenuW(m_ViewMenu, MF_STRING, IDM_FILE_FONT, _T("&Font"));

		AppendMenuW(m_MainMenubar, MF_POPUP, (UINT_PTR)m_fileMenu, _T("&File"));
		AppendMenuW(m_MainMenubar, MF_POPUP, (UINT_PTR)IDM_FILE_TAB, _T("&Tab"));
		//AppendMenuW(m_MainMenubar, MF_POPUP, (UINT_PTR)m_TabMenu, _T("&Tab"));
		AppendMenuW(m_MainMenubar, MF_POPUP, (UINT_PTR)m_ViewMenu, _T("&View"));
		AppendMenuW(m_MainMenubar, MF_POPUP, (UINT_PTR)m_AboutMenu, _T("&About"));
		
		SetMenu(hwnd, m_MainMenubar);
	}

	//void TabControl(HWND hwnd)
	//{
	//	TCITEMW tie;
	//	wchar_t text[4];
	//	LRESULT count, id;
	//	INITCOMMONCONTROLSEX icex;
	//
	//	//RECT rc;
	//	//GetClientRect(hwnd, &rc);
	//
	//	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	//	icex.dwICC = ICC_TAB_CLASSES;
	//	InitCommonControlsEx(&icex);
	//
	//	Window::hTab = CreateWindowW(WC_TABCONTROLW, NULL, WS_CHILD | WS_VISIBLE,
	//			0, 0, 400, 400, hwnd, (HMENU)ID_TABCTRL, NULL, NULL);
	//}

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

	LRESULT CALLBACK ChildProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
		case WM_LBUTTONDOWN:
			MessageBox(hwnd, _T("Child Proc Message Handler!"), _T("ChildProc"), MB_OKCANCEL);
			break;
		case WM_DESTROY :
			CloseWindow(hwnd);
			break;
		default :
			return DefWindowProc(hwnd, msg, wparam, lparam);
		}
		return 0;
	}

	LRESULT CALLBACK CommandProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		int val;
		switch (LOWORD(wparam))
		{
			case IDM_FILE_OPEN :
			case IDM_FILE_NEW :
			case IDM_FILE_VIEW :
			case IDM_FILE_FONT :
				MessageBeep(MB_ICONINFORMATION);
				OpenFileWindow(hwnd);
				break;
			case IDM_FILE_TAB :
				//TabControl(hwnd);
				CE::RenderBuffer(&Window::m_buffer);
				OutputDebugStringA("\n----------------------------------\n");
				MessageBeep(MB_ICONINFORMATION);
				break;
			case IDM_FILE_SAVE :
			case IDM_FILE_SAVE_AS :
				MessageBeep(MB_ICONINFORMATION);
				break;
			case IDM_FILE_QUIT :
				val = MessageBox(hwnd, _T("Do you Want To Quit!"), _T("Quit"), MB_OKCANCEL);
				if(val == IDOK){ return Window::WindowProc(hwnd, WM_DESTROY, wparam, lparam); }
				break;
			case IDM_FILE_ABOUT :
				MessageBox(hwnd, _T("All Rights Reserved To Creator, Kidding! \nUse as you see Fit"), _T("About"), MB_OK | MB_ICONINFORMATION);
				break;
			case IDM_FILE_USAGE :
				MessageBox(hwnd, _T("Given Piece of Software can Malfunction !\n\nThus the Creator is to hold no responsibility under Malfunction !"), _T("User Desgression Required !"), MB_ICONINFORMATION);
				break;
			case IDM_APP_INFO :
				MessageBox(hwnd, _T("Chotu Editor"), _T("Cndr Engine"), MB_OKCANCEL);
				break;
			default :
				return DefWindowProc(hwnd, msg, wparam, lparam);
		}
		return 0;
	}

	void Window::DrawTextToHWND(RECT &rc)
	{
		HRESULT hr;

		BeginDraw();

		m_rendertarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
		D2D1_RECT_F layoutRect;
		layoutRect.left = rc.left;
		layoutRect.right = rc.right;
		layoutRect.top = rc.top;
		layoutRect.bottom = rc.bottom;

		const WCHAR* ch = CharToWchar(m_buffer.data);
		
		m_rendertarget->DrawText(ch, wcslen(ch), m_text_format, layoutRect, m_textBrush);

		hr = EndDraw();
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
			DefMenu(hwnd);
			break;
		case WM_COMMAND :
			return CommandProc(hwnd, msg, wparam, lparam);
		case WM_LBUTTONDOWN:
			break;
		case WM_SIZE :
		{
			Window* m_Window = Window::GetInstance();

			if (!m_Window->m_rendertarget)
			{
				m_Window->m_rendertarget->Release();
			}
			if (!m_Window->m_textBrush)
			{
				m_Window->m_textBrush->Release();
			}
			m_Window->m_d2dfactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hwnd, sz), &m_Window->m_rendertarget);
			hr = m_Window->m_rendertarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_Window->m_textBrush);
			m_Window->DrawTextToHWND(rc);
			break;
		}
		case WM_PAINT :
		case WM_DISPLAYCHANGE :
		{
			Window* m_Window = Window::GetInstance();
			m_Window->DrawTextToHWND(rc);
			break;
		}
		case WM_QUIT:
		case WM_DESTROY :
		case VK_F4 :
			DestroyWindow(hwnd); 
			PostQuitMessage(0);
			Window::m_Running = false;
			break;
		case WM_CHAR:
		{
			char cchar = (static_cast<KeyCode>(wparam));
			if (cchar != CE::Key::Enter && cchar != CE::Key::Backspace)
			{
				std::cout << "Key Pressed! " << cchar << "\n";
				Window::Pos_count = CE::InsertCharacter(&Window::m_buffer, Window::Pos_count, cchar);
				//data->EventCall(event);
				break;
			}
			else if (cchar == CE::Key::Enter)
			{
				std::cout << "Enter Pressed!\n";
				Window::Pos_count = CE::InsertCharacter(&Window::m_buffer, Window::Pos_count, '\n');
				break;
			}
			else if (cchar == CE::Key::Backspace)
			{
				std::cout << "BackSpace Pressed!\n";
				Window::Pos_count = RemoveCharacter(&Window::m_buffer, Window::Pos_count);
				// Reduce m_buffer->end_pos by 1 or however times backspace is pressed!
				break;
			}
			else if (cchar == CE::Key::Tab)
			{
				std::cout << "Tab Pressed\n";
				Window::Pos_count = RemoveCharacter(&Window::m_buffer, Window::Pos_count);
				break;
			}
			break;
		}
		default :
			return DefWindowProc(hwnd, msg, wparam, lparam);
		}
		return 0;
	}

	void Window::ChangeFormat()
	{
		HRESULT hr;
		hr = m_writeFactory->CreateTextFormat(L"Consolas", 0, DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 36.0f, L"en-us", &m_text_format);
		if (hr != S_OK)
		{
			std::cout << "ChangeFormat Didnt Work Properly\n";
		}
	}

	bool Window::Init()
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

		m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, wc.lpszClassName, _T("Cndr Engine"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720,
			NULL, NULL, NULL, NULL);

		if (!m_hwnd)
		{
			return false;
		}

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