#pragma once

#ifdef INFINITY_WINDOWS

#pragma comment(lib, "user32.lib")

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#undef CreateWindow

#include"Core.h"
#include"Window.h"
#include"event/InputCodes.h"

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11DepthStencilState;
struct ID3D11Texture2D;

namespace Infinity
{
	class WindowsContext;
	class Event;

	class INFINITY_API WindowsWindow : public Window
	{
	public:
		struct WindowsWindowContext
		{
			WindowsWindow *window;
			WindowsContext *context;
			ID3D11Device *device;
			ID3D11DeviceContext *device_context;
			ID3D11RenderTargetView *render_target_view;
			ID3D11DepthStencilView *depth_stencil_view;
		};

	private:
		HINSTANCE m_instance;
		HWND m_window_handle;
		DWORD m_style, m_style_ex;

		bool m_should_close;
		RECT m_restored_state;
		bool m_cursor_active;
		unsigned int m_width, m_height, m_prev_width, m_prev_height;

		bool m_vsync, m_fullscreen;
		
		IDXGISwapChain *m_swap_chain;
		ID3D11Device *m_device;
		ID3D11DeviceContext *m_device_context;
		ID3D11RenderTargetView *m_render_target_view;
		ID3D11DepthStencilView *m_depth_stencil_view;
		ID3D11DepthStencilState *m_depth_stencil_state;
		ID3D11Texture2D *m_depth_stencil_buffer;

		WindowsWindowContext m_context;

		bool m_keys[(unsigned int)KeyCode::Last];
		bool m_buttons[(unsigned int)MouseCode::Last];

		bool m_cursor_enabled;
		int m_cursor_x, m_cursor_y, m_restore_cursor_x, m_restore_cursor_y;
		unsigned int m_raw_input_size;
		void *m_raw_input_data;

	public:
		WindowsWindow();
		~WindowsWindow();

		bool Init(const WindowParams &params) override;
		void Destroy() override;

		void MakeContextCurrent() override;

		void Show() override;

		bool ShouldClose() override;
		void SwapBuffers() override;

		bool KeyDown(KeyCode key) const override;
		bool KeyPressed(KeyCode key) const override;
		bool KeyReleased(KeyCode key) const override;
		bool MouseDown(MouseCode button) const override;
		bool MousePressed(MouseCode button) const override;
		bool MouseReleased(MouseCode button) const override;

		void EnableCursor() override;
		void DisableCursor() override;

		bool CursorEnabled() const override;

		int GetCursorPosX() const override;
		int GetCursorPosY() const override;

		static void EventListener(Event *event);

	private:
		bool Resize();
		
		static LRESULT CALLBACK WindowProcedure(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param);
	};
}

#endif // INFINITY_WINDOWS