#pragma once

#include <d2d1_3.h>
#include <dwrite_3.h>
#include <d2d1helper.h>

#include <wrl.h>
#include <string>

namespace
{
	using namespace Microsoft::WRL;
}

namespace CE
{
	struct DrawTextInfo
	{
		D2D1::ColorF renderTargetClearColor;

		D2D1_RECT_F layoutRect;

		const WCHAR* textBuffer;

		DrawTextInfo(D2D1::ColorF clearRTColor, D2D1_RECT_F layoutSize, const WCHAR* buffer)
			:renderTargetClearColor(clearRTColor), layoutRect(layoutSize), textBuffer(buffer)
		{}
	};

	class Renderer
	{
	public :
		Renderer(HWND handle, float scrollOffset = 10.0f, float fontSize = 10.0f);
		~Renderer();

		void OnResize(uint16_t width, uint16_t height);

		void HandleRenderText(DrawTextInfo info);
		void HandleRenderCarat();
		void SetTextColor(D2D1::ColorF color = D2D1::ColorF::Black);

	private :

		bool CreateDirectWriteFactory();
		bool Create2DFactory();
		bool Create2DContext();
		bool Create2DRenderTarget();

		bool CreateResources();

		ComPtr<IDWriteTextFormat> CreateDWriteTextFormat(float fontSize,
			std::wstring_view fontFamily = L"Consolas",
			DWRITE_FONT_WEIGHT  fontWeight = DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE   fontStyle = DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL
		);

		ComPtr<IDWriteTextLayout> CreateDWriteTextLayout(std::wstring text, UINT textLen, float maxWidth = 0.0f, float maxHeight = 0.0f);

	private :
		HWND m_handle;
		RECT m_surfaceRect;

		float m_fontSize;
		float m_scrollOffset;

		std::wstring m_text;

	private :
		ComPtr<ID2D1Factory> m_2dFactory;
		ComPtr<IDWriteFactory> m_writeFactory;
		ComPtr<ID2D1DeviceContext> m_2dContext;
		ComPtr<ID2D1HwndRenderTarget> m_2dRenderTarget;

		ComPtr<IDWriteTextLayout> m_textLayout;

		ComPtr<IDWriteTextFormat> m_textFormat;
		ComPtr<ID2D1SolidColorBrush> m_textBrush;
	};
}