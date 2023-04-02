#pragma once
#include "Base.h"

#include <filesystem>

namespace CE
{
	static std::filesystem::path def_path;

	class File
	{
	public :
		File() = delete;
		File(HWND);

		void LoadFile(const char* path);
		void SaveFile();
		void SetWindowHandle(HWND hwnd) { m_Window = hwnd; }
		bool IsSaved() { return is_saved; }
	public :
		HWND m_Window;
		std::filesystem::path this_file_savepath;
		bool is_saved;
	};

}