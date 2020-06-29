#pragma once

namespace Infinity
{
	class Application;

	void INFINITY_API ExecuteMain(Application *application);
}

#ifdef INFINITY_ENTRY_POINT

extern Infinity::Application *Infinity::CreateApplication();

int main(int argc, char **argv)
{
#ifdef DEBUG
	Infinity::Log::Init();
#endif // DEBUG

	Infinity::ExecuteMain(Infinity::CreateApplication());
}

#endif // INFINITY_ENTRY_POINT