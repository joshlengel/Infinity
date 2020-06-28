#pragma once

#include"Core.h"

#include"event/InputCodes.h"

#include"Input.h"
#include"Window.h"

namespace Infinity
{
	class Context;

	class INFINITY_API BaseWindow : public Window
	{
	public:
		enum class BaseWindowStyle
		{
			MainWindow,
			ChildWindow,
			FullscreenWindow
		};

		struct INFINITY_API BaseWindowParams
		{
			BaseWindow *parent;
			BaseWindowStyle style;

			const char *title;

			unsigned int x;
			unsigned int y;
			unsigned int width;
			unsigned int height;

			bool vsync;
			
			bool auto_swap_buffers;

			// These flags only have an effect on the main window
			bool auto_show;
			bool fullscreen;
			bool enable_alt_enter_fullscreen;

			WindowIcon *icon;
		};

	protected:
		bool m_auto_swap_buffers;
		Context *m_context;
		
		InputDeviceState m_device_states[(unsigned int)InputDevice::Last];

	public:
		BaseWindow();
		virtual ~BaseWindow();

		static bool Init();
		static void PollInput();

		virtual bool Init(const BaseWindowParams &params) = 0;
		virtual void Destroy() = 0;

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

		static BaseWindow *CreateBaseWindow();
	};
}