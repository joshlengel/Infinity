#pragma once

#include"Core.h"

#include"event/InputCodes.h"

#include"Input.h"

namespace Infinity
{
	class Context;

	class INFINITY_API WindowIcon
	{
	public:
		virtual bool Init(const char *file_path) = 0;
		virtual void Destroy() = 0;

		static WindowIcon *CreateWindowIcon();
	};
	
	class INFINITY_API Window
	{
	public:
		struct INFINITY_API MainWindowParams
		{
			const char *title = "Infinity Game Engine";

			unsigned int x = INFINITY_DONT_CARE;
			unsigned int y = INFINITY_DONT_CARE;
			unsigned int width = INFINITY_DONT_CARE;
			unsigned int height = INFINITY_DONT_CARE;

			bool fullscreen = false;
			bool enable_alt_enter_fullscreen = false;
			bool vsync = true;

			bool auto_show = true;
			bool auto_swap_buffers = true;

			WindowIcon *icon = nullptr;
		};

		struct INFINITY_API ChildWindowParams
		{
			const char *title = "Infinity Game Engine";

			unsigned int x = INFINITY_DONT_CARE;
			unsigned int y = INFINITY_DONT_CARE;
			unsigned int width = INFINITY_DONT_CARE;
			unsigned int height = INFINITY_DONT_CARE;

			bool vsync = true;

			bool auto_swap_buffers = true;

			WindowIcon *icon = nullptr;
		};

	protected:
		static Context *s_context;

	public:
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

		static Context *GetContext();
	};
}