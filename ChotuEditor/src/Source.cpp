#include "Chotupch.h"
#include "Application.h"

void ClearResources()
{
}

int main(int argc, char** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	CE::Application* app = new CE::Application();
	app->Run();
	ClearResources();
	delete app;

	return 0;
}