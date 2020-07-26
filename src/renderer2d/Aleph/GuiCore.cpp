#include"InfinityPCH.h"

#include"renderer2d/Aleph/GuiCore.h"

#include"Log.h"

#include"event/Event.h"

#include"application/BaseApplication.h"

namespace Infinity::Aleph
{
	GuiCore::GuiCore() noexcept:
		m_renderer(),
		m_window()
	{}

	GuiCore::~GuiCore()
	{}

	bool GuiCore::Init()
	{
		BaseApplication::GetApplication()->AddPriorityEventHandler(INFINITY_TO_STATIC_EVENT_FUNC(GuiCore::EventHandler));

		// Find out which window made it's context current
		auto window_view = Window::GetContext()->GetWindow();

		if (window_view)
		{
			m_window = Resource<Window>(window_view);
		}

		m_renderer = Renderer2D::CreateRenderer2D();

		if (!m_renderer->Init())
		{
			INFINITY_CORE_ERROR("Error initializing Aleph Gui renderer");
			return false;
		}

		return true;
	}

	void GuiCore::Render()
	{
		m_renderer->StartScene(nullptr);

		for (Resource<Component> &component : m_components) component->Render(m_renderer);

		m_renderer->EndScene();
	}

	void GuiCore::Update()
	{
		for (Resource<Component> &component : m_components) component->Update();
	}

	void GuiCore::EventHandler(Event &event)
	{
		if (event.GetType() == Event::EventType::WindowResized)
		{
			WindowResizedEvent &wre = (WindowResizedEvent&)event;

			if (wre.GetWindow() == m_window)
				for (Resource<Component> &component : m_components) component->SetWindowSize(wre.GetWidth(), wre.GetHeight(), float(wre.GetWidth()) / float(wre.GetHeight()));
		}
	}

	void GuiCore::AddComponent(const Resource<Component> &component)
	{
		m_components.Add(component);
	}

	void GuiCore::RemoveComponent(const Resource<Component> &component)
	{
		Remove(m_components, component);
	}

	Resource<GuiCore> GuiCore::CreateGuiCore()
	{
		return MakeResource<GuiCore>();
	}
}