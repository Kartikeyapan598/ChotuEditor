#include "Chotupch.h"
#include "Renderer.h"

#include <comdef.h>
#include <d2d1helper.h>

#define error(x) std::cout<<x<<" Error :- File: "<<__FILE__<<" | Line: "<<__LINE__<<" | Function: "<<__FUNCTION__<<'\n'

namespace CE
{
	HRESULT hr{ S_OK };

	Renderer::Renderer(HWND handle, float scrollOffset, float fontSize)
		:m_handle(handle), m_scrollOffset(scrollOffset), m_fontSize(fontSize)
	{
		CreateResources();
	}
	Renderer::~Renderer()
	{}

	void Renderer::HandleRenderText(DrawTextInfo info)
	{
		if (m_2dRenderTarget)
		{
			m_2dRenderTarget->BeginDraw();

			D2D1_COLOR_F color = D2D1::ColorF(info.renderTargetClearColor);
			m_2dRenderTarget->Clear(color);
			
			m_2dRenderTarget->DrawText(info.textBuffer, wcslen(info.textBuffer), m_textFormat.Get(), info.layoutRect, m_textBrush.Get());

			m_2dRenderTarget->EndDraw();
		}
		else
		{
			error("No RenderTarget");
		}
	}

	void Renderer::HandleRenderCarat()
	{
		float cursorX, cursorY;
		float offSet = 1.0f;
		DWRITE_HIT_TEST_METRICS cursorMetrics;
		if (!m_textLayout)
		{
			CreateDWriteTextLayout(L"\a", 1);
		}
		m_textLayout->HitTestTextPosition(1, 0, &cursorX, &cursorY, &cursorMetrics);
		
		ComPtr<ID2D1DeviceContext> deviceContext;
		m_2dRenderTarget->QueryInterface(IID_PPV_ARGS(&deviceContext));
		
		ComPtr<ID2D1SolidColorBrush> caratBrush;
		m_2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Tomato), &caratBrush);

		deviceContext->DrawLine(
			{ cursorX + m_fontSize * 1, offSet + cursorY },
			{ cursorX + m_fontSize * 1,offSet + cursorY + cursorMetrics.height },
			caratBrush.Get(),
			m_fontSize / 13
		);
	}

	void Renderer::OnResize(uint16_t width, uint16_t height)
	{
		if (m_2dRenderTarget)
		{
			m_2dRenderTarget->Resize({ width, height });
		}
	}

	void Renderer::SetTextColor(D2D1::ColorF color)
	{
		hr = { S_OK };
		hr = m_2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color), &m_textBrush);
		if (FAILED(hr))
		{
			error("CreateSolidColorBrush");
		}
	}

	ComPtr<IDWriteTextFormat> Renderer::CreateDWriteTextFormat(float fontSize, std::wstring_view fontFamily, DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_STRETCH fontStretch)
	{
		ComPtr<IDWriteTextFormat> textFormat;
		if (m_writeFactory)
		{
			m_writeFactory->CreateTextFormat(fontFamily.data(), NULL, fontWeight, fontStyle, fontStretch, fontSize, L"en-us", &textFormat);
		}
		DWRITE_TRIMMING trimmer{ DWRITE_TRIMMING_GRANULARITY_NONE, 0, 0 };
		
		textFormat->SetTrimming(&trimmer, NULL);
		textFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
		textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

		return textFormat;
	}

	ComPtr<IDWriteTextLayout> Renderer::CreateDWriteTextLayout(std::wstring text, UINT textLen, float maxWidth, float maxHeight)
	{
		DWRITE_TEXT_RANGE DWriteRange{ 0, textLen };

		this->m_writeFactory->CreateTextLayout(
			text.c_str(),
			textLen,
			m_textFormat.Get(),
			maxWidth,
			maxHeight,
			m_textLayout.ReleaseAndGetAddressOf()
		);
		m_textLayout->SetFontSize(m_fontSize, DWriteRange);
		return m_textLayout;
	}

	bool Renderer::CreateDirectWriteFactory()
	{
		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)(& m_writeFactory));
		if (FAILED(hr))
		{
			error("DWriteCreateFactory");
			return false;
		}
		return true;
	}
	
	bool Renderer::Create2DFactory()
	{
		hr = S_OK;
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_2dFactory.ReleaseAndGetAddressOf());
		if (FAILED(hr))
		{
			error("D2D1CreateFactory");
			return false;
		}
		return true;
	}
	
	bool Renderer::Create2DContext()
	{
		m_2dRenderTarget->QueryInterface(IID_PPV_ARGS(&m_2dContext));
		return true;
	}
	
	bool Renderer::Create2DRenderTarget()
	{
		D2D1_SIZE_U size = D2D1::SizeU(m_surfaceRect.right - m_surfaceRect.left,
			m_surfaceRect.bottom - m_surfaceRect.top);
		hr = S_OK;
		hr = m_2dFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_handle, size),
			m_2dRenderTarget.ReleaseAndGetAddressOf());
		if (FAILED(hr))
		{
			_com_error err(hr);
			LPCTSTR errMsg = err.ErrorMessage();
			std::wcout << errMsg << '\n';
			error("CreateHwndRenderTarget");
			return false;
		}
		return true;
	}

	bool Renderer::CreateResources()
	{
		CreateDirectWriteFactory();
		Create2DFactory();
		Create2DRenderTarget();
		m_textFormat = CreateDWriteTextFormat(m_fontSize);
		return true;
	}
}