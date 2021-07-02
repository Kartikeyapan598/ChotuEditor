#include "Chotupch.h"
#include "Application.h"

namespace CE
{
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>((Window*)Window::Create());
	}
	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Window->m_Running)
		{
			m_Window->broadcast();
		}
	}

}