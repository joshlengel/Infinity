#pragma once

#include"Core.h"

#include"event/InputCodes.h"

namespace Infinity
{
	struct INFINITY_API WindowParams
	{
		const char *title = "Infinity Game Engine";

		unsigned int x = INFINITY_DONT_CARE;
		unsigned int y = INFINITY_DONT_CARE;
		unsigned int width = INFINITY_DONT_CARE;
		unsigned int height = INFINITY_DONT_CARE;

		bool vsync = true;
		bool fullscreen = false;
	};

	class INFINITY_API Window
	{
	protected:
		static void *native_context;

	public:
		Window();
		virtual ~Window();

		static void InitListeners();

		virtual bool Init(const WindowParams &params) = 0;
		virtual void Destroy() = 0;

		virtual void MakeContextCurrent() = 0;

		virtual void Show() = 0;

		virtual bool ShouldClose() = 0;

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

		static Window *CreateWindow();
		static void *GetNativeContext();
	};
}