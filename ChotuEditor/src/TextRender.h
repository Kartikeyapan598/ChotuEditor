#pragma once
#include "Base.h"
#include <Windows.h>

typedef uint32_t ui32;

namespace CE
{
	struct Buffer
	{
		char* data;
		//ui32 cursor_pos;
		ui32 gap_start_pos;
		ui32 gap_end_pos;
		ui32 end_pos;
	};

	ui32 CopyLineBuffer(char* dest, Buffer* buffer, int char_count);
	void InitBuffer(Buffer* buffer, int initial_gap_size);
	void ShiftGapToPosition(Buffer* buffer, ui32 position);
	void GapSizeCheck(Buffer* buffer, ui32 minimun_gap_size);
	ui32 InsertCharacter(Buffer* buffer, ui32 position, char chara);
	ui32 RemoveCharacter(Buffer* buffer, ui32 position);
	void RenderBuffer(Buffer* buffer);

	const WCHAR* CharToWchar(char* cchar);

	class TextRender
	{
	public :
		TextRender() { if (!m_Instance) { m_Instance = this; } }
		~TextRender() { delete this; }
		TextRender* Create() { return new TextRender(); }
		static TextRender* GetTextRenderer() { return m_Instance; }
		void InitBuffer(Buffer* buffer, int initial_gap_size);
		void ShiftGapToPosition(Buffer* buffer, ui32 position);
		void GapSizeCheck(Buffer* buffer, ui32 minimun_gap_size);
		void InsertCharacter(Buffer* buffer, ui32 position, char chara);
		void RenderBuffer(Buffer* buffer);

	private :
		Buffer* m_buffer;
		static TextRender* m_Instance;
	};
}