#pragma once

#include"Core.h"

#include"Component.h"

#include"renderer2d/Renderer2D.h"
#include"window/Window.h"

namespace Infinity
{
	namespace Aleph
	{
		class GuiCore;
	}

	INFINITY_TEMPLATE template class INFINITY_API Resource<Aleph::GuiCore>;

	class Event;

	namespace Aleph
	{
		class INFINITY_API GuiCore
		{
			template <typename T, typename ...Args>
			friend Resource<T> Infinity::MakeResource(Args&&... args);

		private:
			Resource<Renderer2D> m_renderer;
			Resource<Window> m_window;

			ArrayList<Resource<Component>> m_components;

			GuiCore() noexcept;
			
		public:
			~GuiCore();

		private:
			void EventHandler(Event &event);

		public:
			bool Init();

			void Render();
			void Update();

			void AddComponent(const Resource<Component> &component);
			void RemoveComponent(const Resource<Component> &component);

			static Resource<GuiCore> CreateGuiCore();
		};
	}
}