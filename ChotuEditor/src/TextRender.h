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

		Buffer() {}
		~Buffer() {}
	};

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
	
	ui32 CopyLineBuffer(char* dest, Buffer* buffer, int char_count)
	{
		return 0;
	}

	void InitBuffer(Buffer* buffer, int initial_gap_size)
	{
		buffer->data = (char*)HeapAlloc(GetProcessHeap(), 0, initial_gap_size);			// Specific for windows
		//buffer->data = (char*)malloc(initial_gap_size);
		//buffer->cursor_pos = 0;
		buffer->gap_start_pos = 0;
		buffer->gap_end_pos = initial_gap_size;
		buffer->end_pos = initial_gap_size;
	}


	void ShiftGapToPosition(Buffer* buffer, ui32 position)
	{
		ui32 gap_size = buffer->gap_end_pos - buffer->gap_start_pos;
		if (position < buffer->gap_start_pos)
		{
			ui32 delta = buffer->gap_start_pos - position;
			buffer->gap_start_pos -= delta;
			buffer->gap_end_pos -= delta;
			MoveMemory(buffer->data + buffer->gap_end_pos , buffer->data + buffer->gap_start_pos, delta);	// memmove((Destination),(Source),(Length)) // memmove((buff->data + buff->gap_end_pos - delta),(buff->data + position),(delta))
			//memmove((buffer->data + buffer->gap_start_pos - delta), (buffer->data + position), (delta));	//	Portable Code
		}
		else if (position > buffer->gap_start_pos)
		{
			ui32 delta = position - buffer->gap_start_pos;
			MoveMemory(buffer->data + buffer->gap_start_pos, buffer->data + buffer->gap_end_pos, delta);
			//memmove((buffer->data + buffer->gap_start_pos), (buffer->data + position), (delta));		//	Portable Code
			buffer->gap_start_pos += delta;
			buffer->gap_end_pos += delta;
		}
	}

	void GapSizeCheck(Buffer* buffer, ui32 minimun_gap_size)
	{
		ui32 gap_size = buffer->gap_end_pos - buffer->gap_start_pos;
		if (gap_size < minimun_gap_size)
		{
			ShiftGapToPosition(buffer, buffer->end_pos - gap_size);
			ui32 new_end_pos = 2 * buffer->end_pos;
			buffer->data = (char*)HeapReAlloc(GetProcessHeap(), 0, buffer->data, new_end_pos);		// Specific for windows
			//buffer->data = (char*)realloc(buffer->data, new_end_pos);
			buffer->gap_end_pos = new_end_pos;
			buffer->end_pos = new_end_pos;
		}
	}

	void InsertCharacter(Buffer* buffer, ui32 position, char chara)
	{
		GapSizeCheck(buffer, 1);
		ShiftGapToPosition(buffer, position);
		buffer->data[buffer->gap_start_pos] = chara;
		buffer->gap_start_pos++;
		//buffer->cursor_pos++;
	}

	void RenderBuffer(Buffer* buffer)
	{
		char temp[1024];
		CopyMemory(temp, buffer->data, buffer->gap_start_pos);
		//memcpy((temp), (buffer->data), (buffer->gap_start_pos));
		temp[buffer->gap_start_pos] = 0;
		OutputDebugStringA(temp);				// Windows only code add alternatives
	}
}
















//class Window;

//namespace CE 
//{
//
//	class TextRender
//	{
//	public :
//		TextRender();
//		~TextRender();
//		static TextRender* Create();
//		void ClearResources();
//		inline IDWriteFactory* GetWriteFactory() { return m_dwite_factory; }
//	public :
//		IDWriteFactory* m_dwite_factory;
//		static inline TextRender* m_Instance;
//	private :
//		friend class Window;
//	};
//
//}