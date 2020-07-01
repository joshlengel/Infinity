#include"InfinityPCH.h"

#include"application/Application.h"
#include"application/BaseApplication.h"

#include"event/Event.h"

namespace Infinity
{
	Application::Application() {}
	Application::~Application() {}

	void Application::RequestExit()
	{
		BaseApplication::GetApplication()->RequestExit();
	}

	void Application::OnUserCreate(UserCreateEvent &event) {}
	void Application::OnUserUpdate(UserUpdateEvent &event) {}
	void Application::OnUserRender(UserRenderEvent &event) {}
	void Application::OnUserDestroy(UserDestroyEvent &event) {}

	void Application::OnApplicationEntered(ApplicationEnteredEvent &event) {}
	void Application::OnApplicationExited(ApplicationExitedEvent &event) {}

	void Application::OnWindowResized(WindowResizedEvent &event) {}
	void Application::OnWindowClosed(WindowClosedEvent &event) {}

	void Application::OnKeyPressed(KeyPressedEvent &event) {}
	void Application::OnKeyReleased(KeyReleasedEvent &event) {}
	void Application::OnMousePressed(MousePressedEvent &event) {}
	void Application::OnMouseReleased(MouseReleasedEvent &event) {}

	void Application::OnCursorEntered(CursorEnteredEvent &event) {}
	void Application::OnCursorExited(CursorExitedEvent &event) {}
	void Application::OnCursorMoved(CursorMovedEvent &event) {}

	const WindowSystem &Application::GetWindowSystem() const { return BaseApplication::GetApplication()->GetWindowSystem(); }
}