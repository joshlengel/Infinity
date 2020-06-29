#pragma once

#include"Core.h"

#include"event/EventQueue.h"
#include"event/InputCodes.h"

#include"utils/data/ArrayList.h"

#include"window/WindowSystem.h"

#include<functional>

namespace Infinity
{
	class Event;
	class WindowResizedEvent;
	class WindowClosedEvent;
	class ApplicationEnteredEvent;
	class ApplicationExitedEvent;
	class UserCreateEvent;
	class UserUpdateEvent;
	class UserRenderEvent;
	class UserDestroyEvent;
	class KeyPressedEvent;
	class KeyReleasedEvent;
	class MousePressedEvent;
	class MouseReleasedEvent;
	class CursorEnteredEvent;
	class CursorExitedEvent;
	class CursorMovedEvent;
	
	class INFINITY_API Application
	{
	public:
		Application();
		virtual ~Application();

		void RequestExit();

		virtual void OnUserCreate(UserCreateEvent *event);
		virtual void OnUserUpdate(UserUpdateEvent *event);
		virtual void OnUserRender(UserRenderEvent *event);
		virtual void OnUserDestroy(UserDestroyEvent *event);

		virtual void OnApplicationEntered(ApplicationEnteredEvent *event);
		virtual void OnApplicationExited(ApplicationExitedEvent *event);

		virtual void OnWindowResized(WindowResizedEvent *event);
		virtual void OnWindowClosed(WindowClosedEvent *event);

		virtual void OnKeyPressed(KeyPressedEvent *event);
		virtual void OnKeyReleased(KeyReleasedEvent *event);
		virtual void OnMousePressed(MousePressedEvent *event);
		virtual void OnMouseReleased(MouseReleasedEvent *event);

		virtual void OnCursorEntered(CursorEnteredEvent *event);
		virtual void OnCursorExited(CursorExitedEvent *event);
		virtual void OnCursorMoved(CursorMovedEvent *event);
		
		const WindowSystem &GetWindowSystem() const;
	};

	extern Application *CreateApplication();
}