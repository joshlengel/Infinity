#pragma once

#ifdef INFINITY_WINDOWS

#pragma comment(lib, "user32.lib")

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#undef CreateWindow

#include"Core.h"
#include"window/BaseWindow.h"

#include"event/Event.h"
#include"event/InputCodes.h"

#include"WindowsContext.h"

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11DepthStencilState;
struct ID3D11Texture2D;

namespace Infinity
{
	class WindowsWindowIcon : public WindowIcon
	{
	private:
		HICON m_big_icon_handle;
		HICON m_small_icon_handle;

	public:
		WindowsWindowIcon();
		~WindowsWindowIcon();

		bool Init(const String &file_path) override;

		HICON GetBigHICON() const;
		HICON GetSmallHICON() const;
	};

	class WindowsWindow : public BaseWindow, public ResourceFromThis<WindowsWindow>
	{
	private:
		HWND m_window_handle;
		DWORD m_style, m_style_ex;

		unsigned int m_x, m_y, m_width, m_height, m_prev_width, m_prev_height;
		RECT m_restored_state;
		bool m_minimized;

		bool m_should_close;
		bool m_showing;

		bool m_vsync;
		bool m_fullscreen, m_enable_alt_enter_fullscreen;
		
		bool m_cursor_active;
		bool m_cursor_enabled;
		int m_cursor_x, m_cursor_y, m_restore_cursor_x, m_restore_cursor_y;

		unsigned int m_raw_input_size;
		void *m_raw_input_data;

		IDXGISwapChain *m_swap_chain;
		ID3D11Device *m_device;
		ID3D11DeviceContext *m_device_context;
		ID3D11RenderTargetView *m_render_target_view;
		ID3D11DepthStencilView *m_depth_stencil_view;
		ID3D11DepthStencilState *m_depth_stencil_state;
		ID3D11Texture2D *m_depth_stencil_buffer;

	public:
		WindowsWindow();
		~WindowsWindow();

		bool Init(const BaseWindowParams &params) override;

		void Show() override;
		void Hide() override;

		bool Showing() const override;

		void SwapBuffers() override;
		bool ShouldClose() const override;
		
		void SetInputDeviceState(InputDevice device, InputDeviceState state) override;
		InputDeviceState GetInputDeviceState(InputDevice device) const override;

		unsigned int GetX() const override;
		unsigned int GetY() const override;
		unsigned int GetWidth() const override;
		unsigned int GetHeight() const override;

		int GetCursorPosX() const override;
		int GetCursorPosY() const override;

		static void EventHandler(Event &event);
		static void HotKeyHandler();

		static LRESULT CALLBACK WindowProcedure(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param);

	private:
		Resource<Window> GetBaseResource() const;
		
		bool Resize();
		
		void UpdateClipRect();

		void EnterFullscreenMode();
		void ExitFullscreenMode();
	};
}

#endif // INFINITY_WINDOWS