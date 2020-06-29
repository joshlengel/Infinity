#include"InfinityPCH.h"

#include"EntryPoint.h"

#include"application/Application.h"
#include"application/BaseApplication.h"

namespace Infinity
{
	void ExecuteMain(Application *application)
	{
		BaseApplication base_app(application);

		base_app.Run();
	}
}