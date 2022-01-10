#pragma once
#include "TextRender.h"
#include "Event.h"

#include <functional>

//static ID2D1Factory* m_d2dfactory;
//static IDWriteFactory* m_writeFactory;
//static ID2D1HwndRenderTarget* m_rendertarget;
//static IDWriteTextFormat* m_text_format;
//static ID2D1SolidColorBrush* m_textBrush;

static WCHAR text[] = L"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

namespace CE
{
	struct Type_Caret
	{
		DWRITE_HIT_TEST_METRICS m_hitMatrices;
		float caret_X;
		float caret_Y;
		bool isTrailinghit = false;
	};

	using CallBack = std::function<void(Event& e)>;
	class Window
	{
	public :
		Window();
		~Window();
		static Window* GetInstance() { return m_Instance; }
		bool Init();
		void  broadcast();
		static Window* Create();
		void ClearResources();
		void DrawTextToHWND(RECT& rc);
		void ChangeFormat();
		void BeginDraw() { if (m_rendertarget) { m_rendertarget->BeginDraw(); } }
		HRESULT EndDraw() { if (m_rendertarget) { HRESULT hr = m_rendertarget->EndDraw(); return hr; } }
		void* GetWindowHandle() { return m_hwnd; }
		void SetBufferandCount(Buffer &buffer, ui32 pos) { m_buffer = buffer; Pos_count = pos; }

		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	public :
		static inline Window* m_Instance;
		static inline bool m_Running;
		static inline HWND m_hwnd;
		static inline HWND mhwnd;
		static inline HWND hTab;
		ID2D1Factory* m_d2dfactory;
		IDWriteFactory* m_writeFactory;
		ID2D1HwndRenderTarget* m_rendertarget;
		IDWriteTextFormat* m_text_format;
		ID2D1SolidColorBrush* m_textBrush;
		//IDWriteTextLayout* m_textlayout;
		
	public :
		static inline Buffer m_buffer;
		static inline ui32 Pos_count;
		static inline Type_Caret m_caret;	// type Caret
		// https://youtu.be/cgd_qGekPdI?t=10929 careting the editor
		//std::unique_ptr<TextRender> m_TextRender;
	private :
		friend class TextRender;
	};

}