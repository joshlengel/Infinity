#pragma once

#include"utils/data/ArrayList.h"
#include"utils/data/Map.h"
#include"utils/data/Memory.h"
#include"utils/data/Pointer.h"
#include"utils/data/Queue.h"
#include"utils/data/String.h"

#include"utils/math/Math.h"

#include"Log.h"

#include<memory>
#include<chrono>

#ifdef INFINITY_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#include<windowsx.h>

	#include<d3d11.h>
	#include<dxgi.h>
	#include<d3dcompiler.h>

	#undef CreateWindow
#endif // INFINITY_WINDOWS
