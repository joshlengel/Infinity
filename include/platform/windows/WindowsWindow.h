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
	private:
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

		bool m_cursor_enabled;
		int m_cursor_x, m_cursor_y, m_restore_cursor_x, m_restore_cursor_y;
		unsigned int m_raw_input_size;
		void *m_raw_input_data;

		bool m_showing;
		bool m_is_main_window;

		static bool hotkey_registered; // only used for the main (first) window created
		static bool main_window_fullscreened;
		static bool main_window_alt_enter_enabled;

	public:
		static bool keys[(unsigned int)KeyCode::Last];
		static bool buttons[(unsigned int)MouseCode::Last];

		WindowsWindow();
		~WindowsWindow();

		bool Init(const WindowParams &params) override;
		void Destroy() override;

		void Show() override;
		void Hide() override;

		bool Showing() const override;

		void SwapBuffers() override;
		bool ShouldClose() const override;
		
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

		static LRESULT CALLBACK WindowProcedure(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param);

	private:
		bool Resize();
		
		void UpdateClipRect();

		void EnterFullscreenMode();
		void ExitFullscreenMode();
	};
}

#endif // INFINITY_WINDOWS