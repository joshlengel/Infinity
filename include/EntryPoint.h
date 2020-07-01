#pragma once

namespace Infinity
{
	class Application;

	Application *CreateApplication();

	void INFINITY_API ExecuteMain(Application *application);
}

#ifdef INFINITY_ENTRY_POINT

int main(int argc, char **argv)
{
#ifdef DEBUG
	Infinity::Log::Init();
#endif // DEBUG

	Infinity::ExecuteMain(Infinity::CreateApplication());
}

#endif // INFINITY_ENTRY_POINT