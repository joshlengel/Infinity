#pragma once

#include"Core.h"

#include"event/InputCodes.h"

namespace Infinity
{
	enum class INFINITY_API InputDevice
	{
		Cursor,
		Last
	};

	enum class INFINITY_API InputDeviceState
	{
		CursorEnabled,
		CursorDisabled
	};

	bool INFINITY_API KeyDown(KeyCode key);
	bool INFINITY_API KeyPressed(KeyCode key);
	bool INFINITY_API KeyReleased(KeyCode key);
	bool INFINITY_API MouseDown(MouseCode button);
	bool INFINITY_API MousePressed(MouseCode button);
	bool INFINITY_API MouseReleased(MouseCode button);

	namespace _Impl
	{
		extern bool keys[(unsigned int)KeyCode::Last];
		extern bool buttons[(unsigned int)MouseCode::Last];

		void Update();
	}
}