#pragma once

#include"Core.h"

#include"Window.h"
#include"utils/data/ArrayList.h"

namespace Infinity
{
	class Event;

	class INFINITY_API WindowSystem
	{
	private:
		Window *m_main_window;
		mutable ArrayList<Window*> m_child_windows;

	public:
		WindowSystem();
		~WindowSystem();

		static void Init();

		bool InitMainWindow(const Window::MainWindowParams &params);

		Window *GetMainWindow() const;
		const ArrayList<Window*> &GetChildWindows() const;

		Window *CreateChildWindow(const Window::ChildWindowParams &params) const;
		void DestroyChildWindow(Window *window) const;

	private:
		static void EventHandler(Event *event);
	};
}