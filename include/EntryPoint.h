#pragma once

#ifdef INFINITY_ENTRY_POINT

int main(int argc, char **argv)
{
	Infinity::Log::Init();

	Infinity::Application *app = Infinity::CreateApplication();

	app->Run();

	delete app;
}

#endif // INFINITY_ENTRY_POINT