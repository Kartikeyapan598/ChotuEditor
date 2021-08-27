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
		void OnUpdate();
		void Close() { m_Running = false; }
		Application& GetApplication() { return *m_ApplicationInstance; }
		Window& GetWindow() { return *m_Window; }
	private:
		bool m_Running;
		std::unique_ptr<Window> m_Window;
		Application* m_ApplicationInstance;
	private:
		friend int ::main(int argc, char** argv);
	};
}