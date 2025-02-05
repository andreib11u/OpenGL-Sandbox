#include "App.h"

int main()
{
	App& app = App::Get();

	if (app.Init())
	{
		app.Run();
	}

	app.Exit();

	return 0;
}
