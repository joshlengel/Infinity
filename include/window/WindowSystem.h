#pragma once

#include"Core.h"

#include"Window.h"

#include"event/Event.h"

#include"utils/data/ArrayList.h"
#include"utils/data/Resource.h"

namespace Infinity
{
	class INFINITY_API WindowSystem
	{
	private:
		Resource<Window> m_main_window;
		mutable ArrayList<Resource<Window>> m_child_windows;

	public:
		WindowSystem();
		~WindowSystem();

		bool InitMainWindow(const Window::MainWindowParams &params);

		Resource<Window> GetMainWindow() const;
		const ArrayList<Resource<Window>> &GetChildWindows() const;

		Resource<Window> CreateChildWindow(const Window::ChildWindowParams &params) const;
		void DestroyChildWindow(Resource<Window> window) const;

	private:
		void EventHandler(Event &event);
	};
}