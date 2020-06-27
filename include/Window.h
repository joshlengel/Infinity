#pragma once

#include"Core.h"

#include"event/InputCodes.h"

namespace Infinity
{
	class Context;

	class INFINITY_API Window
	{
	public:
		struct INFINITY_API WindowParams
		{
			const char *title = "Infinity Game Engine";

			unsigned int x = INFINITY_DONT_CARE;
			unsigned int y = INFINITY_DONT_CARE;
			unsigned int width = INFINITY_DONT_CARE;
			unsigned int height = INFINITY_DONT_CARE;

			bool vsync = true;
			bool fullscreen = false;

			bool auto_swap_buffers = true;
			
			// These flags only have an effect on the main window
			bool auto_show = true;
			bool enable_alt_enter_fullscreen = true;
		};

	protected:
		static Window *main_window;
		bool m_auto_swap_buffers;

		static Context *context;
		Context *m_context;
		
	public:
		Window();
		virtual ~Window();

		static bool Init();

		virtual bool Init(const WindowParams &params) = 0;
		virtual void Destroy() = 0;

		void MakeContextCurrent();

		virtual void Show() = 0;
		virtual void Hide() = 0;

		virtual bool Showing() const = 0;

		virtual bool ShouldClose() const = 0;

		static void PollInput();
		virtual void SwapBuffers() = 0;

		virtual bool KeyDown(KeyCode key) const = 0;
		virtual bool KeyPressed(KeyCode key) const = 0;
		virtual bool KeyReleased(KeyCode key) const = 0;
		virtual bool MouseDown(MouseCode button) const = 0;
		virtual bool MousePressed(MouseCode button) const = 0;
		virtual bool MouseReleased(MouseCode button) const = 0;

		virtual void EnableCursor() = 0;
		virtual void DisableCursor() = 0;

		virtual bool CursorEnabled() const = 0;

		virtual int GetCursorPosX() const = 0;
		virtual int GetCursorPosY() const = 0;

		bool AutoSwapBuffers() const;

		static Window *CreateWindow();
		static Context *GetContext();
	};
}