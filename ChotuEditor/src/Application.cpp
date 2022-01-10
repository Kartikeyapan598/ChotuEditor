#include "Chotupch.h"
#include "TextRender.h"
#include "Application.h"

namespace CE
{
	static ui32 buffer_pos_count = 0;

	Application::Application()
	{
		m_ApplicationInstance = this;
		m_Window = std::unique_ptr<Window>((Window*)Window::Create());
	}
	Application::~Application()
	{
	}

	void Application::Run()
	{
		CE::Buffer buffer;
		CE::InitBuffer(&buffer, 4);
		buffer_pos_count = CE::InsertCharacter(&buffer, buffer_pos_count, 'a');
		buffer_pos_count = CE::InsertCharacter(&buffer, buffer_pos_count, 'b');
		buffer_pos_count = CE::InsertCharacter(&buffer, buffer_pos_count, '\n');
		buffer_pos_count = CE::InsertCharacter(&buffer, buffer_pos_count, 'x');
		buffer_pos_count = CE::InsertCharacter(&buffer, buffer_pos_count, '\n');
		buffer_pos_count = CE::InsertCharacter(&buffer, buffer_pos_count, 'H');
		
		m_Window->SetBufferandCount(buffer, buffer_pos_count);
		
		CE::RenderBuffer(&buffer);
		OutputDebugStringA("\n");
		while (m_Window->m_Running)
		{
			m_Window->broadcast();
		}
	}

}