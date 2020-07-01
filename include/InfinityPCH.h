#pragma once

#include"utils/data/ArrayList.h"
#include"utils/data/Map.h"
#include"utils/data/Resource.h"
#include"utils/data/Queue.h"
#include"utils/data/String.h"

#include"utils/math/Math.h"

#include"utils/profiling/Timer.h"

#include"Log.h"

#include<memory>
#include<chrono>
#include<fstream>
#include<string>
#include<string_view>
#include<sstream>
#include<functional>

#ifdef INFINITY_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#include<windowsx.h>

	#include<d3d11.h>
	#include<dxgi1_2.h>
	#include<d3dcompiler.h>

	#undef CreateWindow
	#undef GetClassName
#endif // INFINITY_WINDOWS
