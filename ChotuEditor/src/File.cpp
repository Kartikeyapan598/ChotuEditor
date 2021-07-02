#include "Chotupch.h"
#include "File.h"

namespace CE
{
	File::File(HWND hwnd)
	{
		m_Window = hwnd;
		if(is_saved)
		{ }
		else
		{
			this_file_savepath = "";
		}
		
	}

	void File::LoadFile(const char* path)
	{
		is_saved = true;
		this_file_savepath = path;
	}

	void File::SaveFile()
	{

	}
}