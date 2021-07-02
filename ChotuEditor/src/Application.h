#pragma once

#include <memory>

#include "Window.h"

int main(int argc, char** argv);

namespace CE
{
	class Application
	{
	public:
		Application();
		~Application();
		void Run();
	private:
		std::unique_ptr<Window> m_Window;
	private:
		friend int ::main(int argc, char** argv);
	};
}