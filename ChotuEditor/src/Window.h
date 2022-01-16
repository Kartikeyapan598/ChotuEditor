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
	struct WindowData
	{
		std::string m_title;
		unsigned int m_width;
		unsigned int m_height;
		bool VSync;

		void ChangeTitle(std::string title) { m_title = title; }
		void ChangeDim(unsigned int width, unsigned int height) { m_height = height, m_width = width; }

		WindowData(const std::string& str = "Chotu Editor",
			unsigned int width = 1280,
			unsigned int height = 720,
			bool VSync_value = 1)
			:m_title(str), m_width(width), m_height(height), VSync(VSync_value)
		{}
	};

	struct Type_Caret
	{
		DWRITE_HIT_TEST_METRICS m_hitMatrices;
		float caret_X;
		float caret_Y;
		bool isTrailinghit = false;
	};

	struct Font
	{
	public :
		const WCHAR* m_font;
		const WCHAR* m_lang;
		float m_fontsize;
	public :
		void ChangeFont(const WCHAR* font) { m_font = font; }
		void ChangeFontSize(float size) { m_fontsize = size; }
		void ChangeLang(const WCHAR* lang) { m_lang = lang; }
		const WCHAR* GetFont() { return m_font; }
		const WCHAR* GetLang() { return m_lang; }
		float GetSize() { return m_fontsize; }
		
	public : 
		Font()
			:m_font(L"Consolas"), m_lang(L"en-us"), m_fontsize(36.0f)
		{}
		Font(const WCHAR* font, const WCHAR* lang, float size)
			:m_font(font), m_lang(lang), m_fontsize(size)
		{}
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
		void SetTitle(const std::string& title);
		void ChangeFormat(const WCHAR* font, float fontsize, const wchar_t* lang);
		void ChangeFormat(const Font font);
		void BeginDraw() { if (m_rendertarget) { m_rendertarget->BeginDraw(); } }
		HRESULT EndDraw() { if (m_rendertarget) { HRESULT hr = m_rendertarget->EndDraw(); return hr; } }
		void* GetWindowHandle() { return m_hwnd; }
		void SetBufferandCount(Buffer &buffer, ui32 pos) { m_buffer = buffer; Pos_count = pos; }

		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	public :
		Font m_fontinfo;
		WindowData m_data;

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
		bool m_ctrl = false;
		bool m_alt = false;
		bool unsaved = true;
		static inline Buffer m_buffer;
		static inline ui32 Pos_count;
		static inline Type_Caret m_caret;	// type Caret

		// https://youtu.be/cgd_qGekPdI?t=10929 careting the editor
		//std::unique_ptr<TextRender> m_TextRender;
	private :
		friend class TextRender;
	};

}