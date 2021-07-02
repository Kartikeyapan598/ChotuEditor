#include "Chotupch.h"
#include "Application.h"
#include "TextRender.h"

void ClearResources()
{
}

int main(int argc, char** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//CE::Buffer buffer;
	//CE::InitBuffer(&buffer, 4);
	//CE::InsertCharacter(&buffer, 0, 'a');
	//CE::InsertCharacter(&buffer, 1, 'b');
	//CE::InsertCharacter(&buffer, 2, '\n');
	//CE::InsertCharacter(&buffer, 3, 'x');
	//CE::InsertCharacter(&buffer, 4, '\n');
	//CE::InsertCharacter(&buffer, 5, 'H');
	//CE::RenderBuffer(&buffer);
	//OutputDebugStringA("\n");
	//
	//return 0;

	CE::Application* app = new CE::Application();
	app->Run();
	ClearResources();
	delete app;

	return 0;
}