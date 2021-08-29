#include "Chotupch.h"
#include "TextRender.h"

#include <memory>
#include <string>
#include <stdlib.h>
#include <atlstr.h>
#include <atlbase.h>
#include <comutil.h>

namespace CE
{
	const WCHAR* CharToWchar(char* cchar)
	{
		int length = MultiByteToWideChar(CP_ACP, 0, cchar, -1, NULL, 0);
		const WCHAR* wchar = new WCHAR[length];
		MultiByteToWideChar(CP_ACP, 0, cchar, -1, (LPWSTR)wchar, length);
		return wchar;
	}

	ui32 IncreaseCount(ui32& count)
	{
		return ++count;
	}

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
			MoveMemory(buffer->data + buffer->gap_end_pos, buffer->data + buffer->gap_start_pos, delta);	// memmove((Destination),(Source),(Length)) // memmove((buff->data + buff->gap_end_pos - delta),(buff->data + position),(delta))
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

	ui32 InsertCharacter(Buffer* buffer, ui32 position, char chara)
	{
		GapSizeCheck(buffer, 1);
		ShiftGapToPosition(buffer, position);
		buffer->data[buffer->gap_start_pos] = chara;
		buffer->gap_start_pos++;
		//buffer->cursor_pos++;
		return IncreaseCount(position);
	}

	ui32 RemoveCharacter(Buffer* buffer, ui32 position)
	{
		if (buffer->data[buffer->gap_start_pos - 1] != '\0')
		{	
			//NULL Error at 0th position , solve;
			//std::cout << "Position" << position << "\n";
			buffer->data[buffer->gap_start_pos - 1] = '\0';
			buffer->gap_start_pos -= 1;
			buffer->gap_end_pos -= 1;
			buffer->end_pos -= 1;
			return position - 1;
		}
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