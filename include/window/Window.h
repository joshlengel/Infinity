#pragma once

#include"Core.h"

#include"Context.h"

#include"event/InputCodes.h"

#include"Input.h"

#include"utils/data/Resource.h"
#include"utils/data/String.h"

namespace Infinity
{
	class INFINITY_API WindowIcon
	{
	public:
		virtual bool Init(const String &file_path) = 0;

		static Resource<WindowIcon> CreateWindowIcon();
	};

	INFINITY_TEMPLATE template class INFINITY_API Resource<WindowIcon>;
	
	class INFINITY_API Window
	{
	public:
		struct INFINITY_API MainWindowParams
		{
			String title = "Infinity Game Engine";

			unsigned int x = INFINITY_DONT_CARE;
			unsigned int y = INFINITY_DONT_CARE;
			unsigned int width = INFINITY_DONT_CARE;
			unsigned int height = INFINITY_DONT_CARE;

			bool fullscreen = false;
			bool enable_alt_enter_fullscreen = false;
			bool vsync = true;

			bool auto_show = true;
			bool auto_swap_buffers = true;

			Resource<WindowIcon> icon = nullptr;
		};

		struct INFINITY_API ChildWindowParams
		{
			String title = "Infinity Game Engine";

			unsigned int x = INFINITY_DONT_CARE;
			unsigned int y = INFINITY_DONT_CARE;
			unsigned int width = INFINITY_DONT_CARE;
			unsigned int height = INFINITY_DONT_CARE;

			bool vsync = true;

			bool auto_swap_buffers = true;

			Resource<WindowIcon> icon = nullptr;
		};

	protected:
		static Resource<Context> s_context;

	public:
		virtual ~Window() {}

		virtual void MakeContextCurrent() const = 0;

		virtual void Show() = 0;
		virtual void Hide() = 0;

		virtual bool Showing() const = 0;

		virtual bool ShouldClose() const = 0;

		virtual void SwapBuffers() = 0;

		virtual void SetInputDeviceState(InputDevice device, InputDeviceState state) = 0;
		virtual InputDeviceState GetInputDeviceState(InputDevice device) const = 0;

		virtual int GetCursorPosX() const = 0;
		virtual int GetCursorPosY() const = 0;

		virtual unsigned int GetX() const = 0;
		virtual unsigned int GetY() const = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		static Resource<Context> GetContext();
	};

	INFINITY_TEMPLATE template class INFINITY_API Resource<Window>;
}