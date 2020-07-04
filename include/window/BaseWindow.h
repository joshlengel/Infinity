#pragma once

#include"Core.h"

#include"Input.h"

#include"Window.h"
#include"Context.h"

#include"event/InputCodes.h"

#include"utils/data/Resource.h"
#include"utils/data/String.h"

namespace Infinity
{
	class INFINITY_API BaseWindow : public Window
	{
	public:
		enum class BaseWindowStyle
		{
			MainWindow,
			ChildWindow,
			FullscreenWindow
		};

		struct BaseWindowParams
		{
			Resource<BaseWindow> parent = nullptr;
			BaseWindowStyle style = BaseWindowStyle::MainWindow;

			String title = String();

			unsigned int x = INFINITY_DONT_CARE;
			unsigned int y = INFINITY_DONT_CARE;
			unsigned int width = INFINITY_DONT_CARE;
			unsigned int height = INFINITY_DONT_CARE;

			bool vsync = true;
			
			bool auto_swap_buffers = true;

			// These flags only have an effect on the main window
			bool auto_show = true;
			bool fullscreen = false;
			bool enable_alt_enter_fullscreen = false;

			Resource<WindowIcon> icon = nullptr;
		};

	protected:
		bool m_auto_swap_buffers;
		Resource<Context> m_context;
		
		InputDeviceState m_device_states[(unsigned int)InputDevice::Last];

	public:
		BaseWindow();
		virtual ~BaseWindow();

		static bool Init();
		static void PollInput();

		virtual bool Init(const BaseWindowParams &params) = 0;

		virtual void Show() override = 0;
		virtual void Hide() override = 0;

		virtual bool Showing() const override = 0;

		virtual bool ShouldClose() const override = 0;

		virtual void SwapBuffers() override = 0;

		virtual void SetInputDeviceState(InputDevice device, InputDeviceState state) override = 0;
		virtual InputDeviceState GetInputDeviceState(InputDevice device) const override = 0;

		virtual unsigned int GetX() const override = 0;
		virtual unsigned int GetY() const override = 0;
		virtual unsigned int GetWidth() const override = 0;
		virtual unsigned int GetHeight() const override = 0;

		virtual int GetCursorPosX() const override = 0;
		virtual int GetCursorPosY() const override = 0;

		void MakeContextCurrent() const override;

		bool AutoSwapBuffers() const;

		static Resource<BaseWindow> CreateBaseWindow();
	};
}