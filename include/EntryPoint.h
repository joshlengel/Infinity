#pragma once

#ifdef INFINITY_ENTRY_POINT

int main(int argc, char **argv)
{
#ifdef DEBUG
	Infinity::Log::Init();
#endif // DEBUG

	Infinity::Application *app = Infinity::CreateApplication();

	app->Run();

	delete app;
}

#endif // INFINITY_ENTRY_POINT