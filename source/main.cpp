#include "App/App.h"

int main()
{
	App& app = App::Get();

	try
	{
		if (app.Init())
		{
			app.Run();
		}
		app.Exit();
	}
	catch (const std::exception& ex)
	{
		printf("Error: %s", ex.what());
	}

	return 0;
}
