#include"InfinityPCH.h"

#ifdef INFINITY_WINDOWS

#include"platform/windows/WindowsWindow.h"

#include"platform/windows/WindowsContext.h"

#include"application/BaseApplication.h"
#include"event/Event.h"

constexpr static const char CLASS_NAME[] = "Infinity WindowsWindow Class";

static const int FULLSCREEN_HOTKEY_ID = GlobalAddAtomA("FullscreenHotkeyID");

constexpr static const long WINDOWED_STYLE = WS_OVERLAPPEDWINDOW;
constexpr static const long WINDOWED_STYLE_EX = WS_EX_APPWINDOW;
constexpr static const long CHILD_STYLE = WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_CLIPSIBLINGS;
constexpr static const long CHILD_STYLE_EX = WS_EX_APPWINDOW;
constexpr static const long FULLSCREEN_STYLE = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
constexpr static const long FULLSCREEN_STYLE_EX = WS_EX_APPWINDOW | WS_EX_TOPMOST;

static HINSTANCE window_instance = nullptr;
static unsigned int window_instance_count = 0;

static int screen_width = 0;
static int screen_height = 0;

static int refresh_rate_numerator = 0;
static int refresh_rate_denominator = 0;

namespace Infinity
{
	Resource<WindowIcon> WindowIcon::CreateWindowIcon()
	{
		return ResourceCast<WindowIcon>(MakeResource<WindowsWindowIcon>());
	}

	WindowsWindowIcon::WindowsWindowIcon():
		m_big_icon_handle(nullptr),
		m_small_icon_handle(nullptr)
	{}

	WindowsWindowIcon::~WindowsWindowIcon()
	{
		DestroyIcon(m_big_icon_handle);
		DestroyIcon(m_small_icon_handle);
	}

	bool WindowsWindowIcon::Init(const String &file_path)
	{
		m_big_icon_handle = (HICON)LoadImageA(nullptr, &file_path[0], IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
		m_small_icon_handle = (HICON)LoadImageA(nullptr, &file_path[0], IMAGE_ICON, 16, 16, LR_LOADFROMFILE);

		if (!m_big_icon_handle || !m_small_icon_handle) return false;

		return true;
	}

	HICON WindowsWindowIcon::GetBigHICON() const { return m_big_icon_handle; }
	HICON WindowsWindowIcon::GetSmallHICON() const { return m_small_icon_handle; }

	bool BaseWindow::Init()
	{
		window_instance = GetModuleHandle(nullptr);

		if (!window_instance) return false;

		WNDCLASSA wc = {};
		wc.hInstance = window_instance;
		wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

		wc.lpszClassName = CLASS_NAME;
		wc.lpszMenuName = nullptr;
		wc.lpfnWndProc = WindowsWindow::WindowProcedure;

		RegisterClassA(&wc);

		screen_width = GetSystemMetrics(SM_CXSCREEN);
		screen_height = GetSystemMetrics(SM_CYSCREEN);

		// find monitor refresh rate
		IDXGIFactory *factory;
		if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory))) return false;

		IDXGIAdapter *adapter;
		if (FAILED(factory->EnumAdapters(0, &adapter))) return false;

		IDXGIOutput *output;
		if (FAILED(adapter->EnumOutputs(0, &output))) return false;

		unsigned int num_modes;
		if (FAILED(output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &num_modes, nullptr))) return false;

		DXGI_MODE_DESC *modes = new DXGI_MODE_DESC[num_modes];
		if (FAILED(output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &num_modes, modes))) return false;

		for (unsigned int i = 0; i < num_modes; ++i)
		{
			const DXGI_MODE_DESC &mode = modes[i]; // C6385 Warning is nonsense

			if (mode.Width == screen_width && mode.Height == screen_height)
			{
				refresh_rate_numerator = mode.RefreshRate.Numerator;
				refresh_rate_denominator = mode.RefreshRate.Denominator;
			}
		}

		delete[] modes;

		output->Release();
		adapter->Release();
		factory->Release();

		if (!RegisterHotKey(nullptr, FULLSCREEN_HOTKEY_ID, MOD_ALT | MOD_NOREPEAT, VK_RETURN))
		{
			INFINITY_CORE_WARN("Unable to register hotkey for alt+enter");
		}

		BaseApplication::GetApplication()->AddPriorityEventHandler(WindowsWindow::EventHandler);

		return true;
	}

	Resource<BaseWindow> BaseWindow::CreateBaseWindow()
	{
		return ResourceCast<BaseWindow>(MakeResource<WindowsWindow>());
	}

	WindowsWindow::WindowsWindow():
		m_window_handle(nullptr),
		m_style(),
		m_style_ex(),

		m_should_close(false),
		m_showing(false),

		m_x(),
		m_y(),
		m_width(),
		m_height(),
		m_prev_width(),
		m_prev_height(),
		m_restored_state(),
		m_minimized(false),

		m_vsync(),
		m_fullscreen(),
		m_enable_alt_enter_fullscreen(),

		m_cursor_active(),
		m_cursor_enabled(true),
		m_cursor_x(),
		m_cursor_y(),
		m_restore_cursor_x(),
		m_restore_cursor_y(),

		m_raw_input_size(0),
		m_raw_input_data(nullptr),

		m_swap_chain(nullptr),
		m_device(nullptr),
		m_device_context(nullptr),
		m_render_target_view(nullptr),
		m_depth_stencil_buffer(nullptr),
		m_depth_stencil_view(nullptr),
		m_depth_stencil_state(nullptr)
	{}

	WindowsWindow::~WindowsWindow()
	{
		if (s_context == m_context) s_context = nullptr;

		if (m_render_target_view)
		{
			m_render_target_view->Release();
		}

		if (m_depth_stencil_buffer)
		{
			m_depth_stencil_buffer->Release();
		}

		if (m_depth_stencil_state)
		{
			m_depth_stencil_state->Release();
		}

		if (m_depth_stencil_view)
		{
			m_depth_stencil_view->Release();
		}

		if (m_swap_chain)
		{
			m_swap_chain->SetFullscreenState(false, nullptr);

			m_swap_chain->Release();
		}

		if (m_device)
		{
			m_device->Release();
		}

		if (m_device_context)
		{
			m_device_context->Release();
		}

		if (m_raw_input_data)
		{
			free(m_raw_input_data);
		}
	}

	bool WindowsWindow::Init(const BaseWindowParams &params)
	{
		m_vsync = params.vsync;
		m_fullscreen = params.fullscreen;
		m_enable_alt_enter_fullscreen = params.enable_alt_enter_fullscreen;

		m_auto_swap_buffers = params.auto_swap_buffers;
		
		switch (params.style)
		{
		case BaseWindowStyle::MainWindow:
		case BaseWindowStyle::FullscreenWindow: // we change the style to fullscreen later
			m_style = WINDOWED_STYLE;
			m_style_ex = WINDOWED_STYLE_EX;
			break;

		case BaseWindowStyle::ChildWindow:
			m_style = CHILD_STYLE;
			m_style_ex = CHILD_STYLE_EX;
			break;

		default:
			INFINITY_CORE_ERROR("Unsupported window style '{}'", (unsigned int)params.style);
		}

		int x = params.x == INFINITY_DONT_CARE? CW_USEDEFAULT : (signed)params.x;
		int y = params.y == INFINITY_DONT_CARE? CW_USEDEFAULT : (signed)params.y;
		int width = params.width == INFINITY_DONT_CARE? CW_USEDEFAULT : (signed)params.width;
		int height = params.height == INFINITY_DONT_CARE? CW_USEDEFAULT : (signed)params.height;

		HWND parent_window = nullptr;

		if (params.parent)
		{
			parent_window = ResourceCast<WindowsWindow>(params.parent)->m_window_handle;
		}

		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = width;
		rect.bottom = height;

		AdjustWindowRectEx(&rect, m_style, false, m_style_ex);

		m_window_handle = CreateWindowExA(m_style_ex, CLASS_NAME, &params.title[0], m_style,
			x == CW_USEDEFAULT? CW_USEDEFAULT : x + rect.left,
			y == CW_USEDEFAULT? CW_USEDEFAULT : y + rect.top,
			width == CW_USEDEFAULT? CW_USEDEFAULT : rect.right - rect.left,
			height == CW_USEDEFAULT? CW_USEDEFAULT : rect.bottom - rect.top,
			parent_window,
			nullptr, window_instance, nullptr);

		if (!m_window_handle) return false;

		SetWindowLongPtrA(m_window_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

		TRACKMOUSEEVENT tme = {};
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_window_handle;
		TrackMouseEvent(&tme);

		RECT window_rect;
		GetClientRect(m_window_handle, &window_rect);
		ClientToScreen(m_window_handle, (POINT*)&window_rect.left);
		ClientToScreen(m_window_handle, (POINT*)&window_rect.right);

		unsigned int a_x =      (unsigned int) window_rect.left;
		unsigned int a_y =      (unsigned int) window_rect.top;
		unsigned int a_width =  (unsigned int)(window_rect.right - window_rect.left);
		unsigned int a_height = (unsigned int)(window_rect.bottom - window_rect.top);

		m_x = a_x;
		m_y = a_y;
		m_width = a_width;
		m_height = a_height;

		// setup dxgi and direct3d 11
		
#ifdef DEBUG
		UINT flags = D3D11_CREATE_DEVICE_DEBUG;
#else
		UINT flags = 0;
#endif // DEBUG
		constexpr D3D_FEATURE_LEVEL feature_levels[] = { D3D_FEATURE_LEVEL_11_0 };
		constexpr unsigned int num_feature_levels = sizeof(feature_levels) / sizeof(D3D_FEATURE_LEVEL);

		if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags,
			feature_levels, num_feature_levels, D3D11_SDK_VERSION, &m_device, nullptr, &m_device_context))) return false;

		DXGI_SWAP_CHAIN_DESC sc_desc = {};
		sc_desc.BufferCount = 2;
		sc_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		sc_desc.BufferDesc.Width = m_width;
		sc_desc.BufferDesc.Height = m_height;
		sc_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sc_desc.BufferDesc.RefreshRate.Numerator = params.vsync? refresh_rate_numerator : 0;
		sc_desc.BufferDesc.RefreshRate.Denominator = params.vsync? refresh_rate_denominator : 1;
		sc_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
		sc_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		
		sc_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		sc_desc.Windowed = true;
		sc_desc.OutputWindow = m_window_handle;

		sc_desc.SampleDesc.Count = 8;

		for (unsigned int count = sc_desc.SampleDesc.Count; count > 0; --count)
		{
			sc_desc.SampleDesc.Count = count;

			unsigned int levels;
			if (FAILED(m_device->CheckMultisampleQualityLevels(sc_desc.BufferDesc.Format, sc_desc.SampleDesc.Count, &levels)))
				continue;

			if (levels > 0)
			{
				sc_desc.SampleDesc.Quality = levels - 1;
				break;
			}
		}

		if (sc_desc.SampleDesc.Count == 1)
		{
			INFINITY_CORE_WARN("Multisampling not available");
			sc_desc.SampleDesc.Quality = 0;
		}
		else
		{
			INFINITY_CORE_INFO("Graphics implementation supports {0} MSAA samples", sc_desc.SampleDesc.Count);
		}

		sc_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		IDXGIDevice *device_dxgi;
		if (FAILED(m_device->QueryInterface(__uuidof(IDXGIDevice2), (void**)&device_dxgi))) return false;

		IDXGIAdapter *device_adapter;
		if (FAILED(device_dxgi->GetAdapter(&device_adapter))) return false;

		IDXGIFactory *device_factory;
		if (FAILED(device_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&device_factory))) return false;

		if (FAILED(device_factory->CreateSwapChain(m_device, &sc_desc, &m_swap_chain))) return false;

		D3D11_DEPTH_STENCIL_DESC ds_desc = {};
		ds_desc.DepthEnable = true;
		ds_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		ds_desc.DepthFunc = D3D11_COMPARISON_LESS;

		ds_desc.StencilEnable = true;
		ds_desc.StencilReadMask = 0xff;
		ds_desc.StencilWriteMask = 0xff;

		ds_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		ds_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		ds_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		ds_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;

		ds_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		ds_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		ds_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		ds_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;

		if (FAILED(m_device->CreateDepthStencilState(&ds_desc, &m_depth_stencil_state)))
		{
			INFINITY_CORE_ERROR("Error creating depth stencil state");
			return false;
		}

		m_device_context->OMSetDepthStencilState(m_depth_stencil_state, 1);
		
		// get IDXGIFactory

		device_factory->MakeWindowAssociation(m_window_handle, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);

		device_factory->Release();
		device_adapter->Release();
		device_dxgi->Release();

		m_context = ResourceCast<Context>(MakeResource<WindowsContext>(m_device, m_device_context));

		Resource<Context> restore_context = s_context;
		s_context = m_context;

		if (!m_context->Init())
		{
			INFINITY_CORE_ERROR("Error initializing window context");
			return false;
		}

		s_context = restore_context;
		
		if (m_fullscreen)
		{
			Resize();
			EnterFullscreenMode();
		}

		if (params.icon)
		{
			Resource<WindowsWindowIcon> icon = ResourceCast<WindowsWindowIcon>(params.icon);
			SendMessageA(m_window_handle, WM_SETICON, ICON_SMALL, (LPARAM)icon->GetSmallHICON());
			SendMessageA(m_window_handle, WM_SETICON, ICON_BIG, (LPARAM)icon->GetBigHICON());
		}

		BaseApplication::GetApplication()->PushEvent(new WindowResizedEvent(m_width, m_height, GetBaseResource()));

		return true;
	}

	void WindowsWindow::Show()
	{
		ShowWindow(m_window_handle, SW_SHOW);
		m_showing = true;
	}

	void WindowsWindow::Hide()
	{
		ShowWindow(m_window_handle, SW_HIDE);
		m_showing = false;
	}

	bool WindowsWindow::Showing() const
	{
		return m_showing;
	}

	bool WindowsWindow::ShouldClose() const
	{
		return m_should_close;
	}

	void BaseWindow::PollInput()
	{
		MSG msg;

		while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
			case WM_QUIT: return;
			case WM_HOTKEY:
				if (msg.wParam == (unsigned)FULLSCREEN_HOTKEY_ID)
				{
					WindowsWindow::HotKeyHandler();
				}
				break;
			}
			
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}

	void WindowsWindow::SwapBuffers()
	{
		if (m_width == 0 || m_height == 0) return;

		if (m_vsync) m_swap_chain->Present(1, 0);
		else	     m_swap_chain->Present(0, 0);
	}

	void WindowsWindow::UpdateClipRect()
	{
		if (GetInputDeviceState(InputDevice::Cursor) == InputDeviceState::CursorDisabled)
		{
			RECT client_rect;
			GetClientRect(m_window_handle, &client_rect);
			ClientToScreen(m_window_handle, (POINT*)&client_rect.left);
			ClientToScreen(m_window_handle, (POINT*)&client_rect.right);
			ClipCursor(&client_rect);
		}
		else
		{
			ClipCursor(nullptr);
		}
	}

	void WindowsWindow::SetInputDeviceState(InputDevice device, InputDeviceState state)
	{
		m_device_states[(unsigned int)device] = state;

		switch (device)
		{
		case InputDevice::Cursor:

			switch (state)
			{
			case InputDeviceState::CursorEnabled:
			{
				m_cursor_enabled = true;

				RAWINPUTDEVICE rid = { 0x01, 0x02, RIDEV_REMOVE, nullptr };

				if (!RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE)))
				{
					INFINITY_CORE_ERROR("Error enabling cursor");
					return;
				}

				m_cursor_x = m_restore_cursor_x;
				m_cursor_y = m_restore_cursor_y;

				POINT point = { m_restore_cursor_x, m_restore_cursor_y };
				ClientToScreen(m_window_handle, &point);

				SetCursorPos(point.x, point.y);

				UpdateClipRect();

				ShowCursor(true);
				break;
			}
			case InputDeviceState::CursorDisabled:
			{
				m_cursor_enabled = false;

				m_restore_cursor_x = m_cursor_x;
				m_restore_cursor_y = m_cursor_y;

				m_cursor_x = 0;
				m_cursor_y = 0;

				ShowCursor(false);

				POINT point = { (LONG)(m_width / 2), (LONG)(m_height / 2) };
				ClientToScreen(m_window_handle, &point);

				SetCursorPos(point.x, point.y);
				UpdateClipRect();

				RAWINPUTDEVICE rid = { 0x01, 0x02, 0, m_window_handle };

				if (!RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE)))
				{
					INFINITY_CORE_ERROR("Error disabling cursor");
					return;
				}
				break;
			}
			default:
			{
				INFINITY_CORE_ERROR("Unknown InputDeviceState '{}'", (unsigned int)state);
				break;
			}
			}

			break;

		default:
			INFINITY_CORE_ERROR("Unknown InputDevice '{}'", (unsigned int)state);
		}
	}

	InputDeviceState WindowsWindow::GetInputDeviceState(InputDevice device) const
	{
		return m_device_states[(unsigned int)device];
	}

	int WindowsWindow::GetCursorPosX() const { return m_cursor_x; }
	int WindowsWindow::GetCursorPosY() const { return m_cursor_y; }

	unsigned int WindowsWindow::GetX() const { return m_x; }
	unsigned int WindowsWindow::GetY() const { return m_y; }
	unsigned int WindowsWindow::GetWidth() const { return m_width; }
	unsigned int WindowsWindow::GetHeight() const { return m_height; }

	Resource<Window> WindowsWindow::GetBaseResource() const { return ResourceCast<Window>(GetResourceFromThis()); }

	bool WindowsWindow::Resize()
	{
		if (m_width != m_prev_width || m_height != m_prev_height)
		{
			m_prev_width = m_width;
			m_prev_height = m_height;

			if (m_width == 0 || m_height == 0)
			{
				INFINITY_CORE_TRACE("Window successfully resized to width: 0, height: 0");
				return true;
			}

			if (m_render_target_view)
			{
				m_render_target_view->Release();
				m_render_target_view = nullptr;
			}

			if (m_depth_stencil_buffer)
			{
				m_depth_stencil_buffer->Release();
				m_depth_stencil_buffer = nullptr;
			}

			if (m_depth_stencil_view)
			{
				m_depth_stencil_view->Release();
				m_depth_stencil_view = nullptr;
			}

			m_device_context->OMSetRenderTargets(1, &m_render_target_view, nullptr);
			
			if (FAILED(m_swap_chain->ResizeBuffers(0, m_width, m_height, DXGI_FORMAT_UNKNOWN, 0)))
			{
				INFINITY_CORE_ERROR("Error resizing window buffers");
				BaseApplication::GetApplication()->PushEvent(new ApplicationInterruptedEvent);
				return false;
			}
			
			ID3D11Texture2D *back_buffer;
			if (FAILED(m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer)))
			{
				INFINITY_CORE_ERROR("Error getting window buffer after resize");
				BaseApplication::GetApplication()->PushEvent(new ApplicationInterruptedEvent);
				return false;
			}

			D3D11_TEXTURE2D_DESC back_buffer_desc;
			back_buffer->GetDesc(&back_buffer_desc);

			D3D11_RENDER_TARGET_VIEW_DESC rtv_desc = {};
			rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
			rtv_desc.Format = back_buffer_desc.Format;

			if (FAILED(m_device->CreateRenderTargetView(back_buffer, &rtv_desc, &m_render_target_view)))
			{
				INFINITY_CORE_ERROR("Error creating render target view after resize");
				back_buffer->Release();
				BaseApplication::GetApplication()->PushEvent(new ApplicationInterruptedEvent);
				return false;
			}

			back_buffer->Release();

			D3D11_TEXTURE2D_DESC dsb_desc = back_buffer_desc;
			dsb_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			dsb_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

			if (FAILED(m_device->CreateTexture2D(&dsb_desc, nullptr, &m_depth_stencil_buffer)))
			{
				INFINITY_CORE_ERROR("Error creating depth stencil buffer after resizing context");
				BaseApplication::GetApplication()->PushEvent(new ApplicationInterruptedEvent);
				return false;
			}

			D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};
			dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
			dsv_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

			if (FAILED(m_device->CreateDepthStencilView(m_depth_stencil_buffer, &dsv_desc, &m_depth_stencil_view)))
			{
				INFINITY_CORE_ERROR("Error creating depth stencil view after resizing context");
				BaseApplication::GetApplication()->PushEvent(new ApplicationInterruptedEvent);
				return false;
			}
			
			Resource<WindowsContext> win_con = ResourceCast<WindowsContext>(m_context);

			if (win_con)
			{
				if (!win_con->Resize(m_render_target_view, m_depth_stencil_view, m_width, m_height))
				{
					INFINITY_CORE_ERROR("Error resizing context");
					BaseApplication::GetApplication()->PushEvent(new ApplicationInterruptedEvent);
					return false;
				}
			}
			else
			{
				INFINITY_CORE_WARN("No context bound after window resized");
				return false;
			}

			INFINITY_CORE_TRACE("Window successfully resized to width: {0}, height: {1}", m_width, m_height);
			
			return true;
		}

		return false;
	}

	void WindowsWindow::EventHandler(Event &event)
	{
		switch (event.GetType())
		{
		case Event::EventType::WindowResized:
		{
			WindowResizedEvent &wre = (WindowResizedEvent&)event;
			Resource<WindowsWindow> window = ResourceCast<WindowsWindow>(wre.GetWindow());

			if (!window->Resize())
				event.Consume();

			break;
		}
		case Event::EventType::WindowClosed:
		{
			WindowClosedEvent &wre = (WindowClosedEvent&)event;
			Resource<WindowsWindow> window = ResourceCast<WindowsWindow>(wre.GetWindow());

			window->m_should_close = true;
			DestroyWindow(window->m_window_handle);
			break;
		}
		}
	}

	void WindowsWindow::EnterFullscreenMode()
	{
		GetWindowRect(m_window_handle, &m_restored_state);

		SetWindowLongA(m_window_handle, GWL_STYLE, FULLSCREEN_STYLE);
		SetWindowLongA(m_window_handle, GWL_EXSTYLE, FULLSCREEN_STYLE_EX);

		m_width = screen_width;
		m_height = screen_height;

		if (FAILED(m_swap_chain->SetFullscreenState(true, nullptr)))
		{
			INFINITY_CORE_ERROR("Error changing to fullscreen state");
			return;
		}

		if (!SetWindowPos(m_window_handle, HWND_TOP, 0, 0, m_width, m_height, SWP_FRAMECHANGED))
		{
			INFINITY_CORE_ERROR("Error resizing window after entering fullscreen mode");
			BaseApplication::GetApplication()->PushEvent(new ApplicationInterruptedEvent);
			return;
		}

		UpdateClipRect();

		ShowWindow(m_window_handle, m_showing? SW_SHOW : SW_HIDE);
	}

	void WindowsWindow::ExitFullscreenMode()
	{
		if (FAILED(m_swap_chain->SetFullscreenState(false, nullptr)))
		{
			INFINITY_CORE_ERROR("Error changing to restored state");
			BaseApplication::GetApplication()->PushEvent(new ApplicationInterruptedEvent);
			return;
		}

		SetWindowLongA(m_window_handle, GWL_STYLE, WINDOWED_STYLE);
		SetWindowLongA(m_window_handle, GWL_EXSTYLE, WINDOWED_STYLE_EX);

		if (!SetWindowPos(m_window_handle, nullptr, m_restored_state.left, m_restored_state.top,
			m_restored_state.right - m_restored_state.left, m_restored_state.bottom - m_restored_state.top,
			SWP_NOZORDER | SWP_NOACTIVATE))
		{
			INFINITY_CORE_ERROR("Error resizing window after exiting fullscreen mode");
			BaseApplication::GetApplication()->PushEvent(new ApplicationInterruptedEvent);
			return;
		}

		UpdateClipRect();

		GetClientRect(m_window_handle, &m_restored_state);

		m_width = m_restored_state.right;
		m_height = m_restored_state.bottom;

		ShowWindow(m_window_handle, m_showing? SW_SHOW : SW_HIDE);
	}

	void WindowsWindow::HotKeyHandler()
	{
		Resource<WindowsWindow> window = ResourceCast<WindowsWindow>(BaseApplication::GetApplication()->GetWindowSystem().GetMainWindow());
		
		if (window->m_enable_alt_enter_fullscreen)
		{
			window->m_fullscreen = !window->m_fullscreen;

			if (window->m_fullscreen)
			{
				for (Resource<Window> window : BaseApplication::GetApplication()->GetWindowSystem().GetChildWindows())
				{
					ShowWindow(ResourceCast<WindowsWindow>(window)->m_window_handle, SW_HIDE);
				}

				window->EnterFullscreenMode();

				BaseApplication::GetApplication()->PushEvent(new WindowResizedEvent(window->m_width, window->m_height, ResourceCast<Window>(window)));
			}
			else
			{
				window->ExitFullscreenMode();

				for (Resource<Window> window : BaseApplication::GetApplication()->GetWindowSystem().GetChildWindows())
				{
					Resource<WindowsWindow> wwin = ResourceCast<WindowsWindow>(window);

					if (wwin->m_showing)
					{
						ShowWindow(wwin->m_window_handle, SW_SHOW);
					}
				}

				BaseApplication::GetApplication()->PushEvent(new WindowResizedEvent(window->m_width, window->m_height, ResourceCast<Window>(window)));
			}
		}
	}

	LRESULT WindowsWindow::WindowProcedure(HWND window_handle, UINT msg, WPARAM w_param, LPARAM l_param)
	{
		WindowsWindow *window = reinterpret_cast<WindowsWindow*>(GetWindowLongPtrA(window_handle, GWLP_USERDATA));

		switch (msg)
		{
		case WM_MOVE:
		{
			window->m_x = (unsigned)LOWORD(l_param);
			window->m_y = (unsigned)HIWORD(l_param);

			return 0;
		}
		case WM_SIZE:
		{
			window->m_width = (unsigned)LOWORD(l_param);
			window->m_height = (unsigned)HIWORD(l_param);

			if (window->m_minimized && (window->m_width != 0 || window->m_height != 0))
			{
				window->m_minimized = false;
				BaseApplication::GetApplication()->PushEvent(new WindowResizedEvent(window->m_width, window->m_height, window->GetBaseResource()));
				return 0;
			}

			switch (w_param)
			{
			case SIZE_MINIMIZED:
				window->m_minimized = true;

			case SIZE_MAXIMIZED:
				BaseApplication::GetApplication()->PushEvent(new WindowResizedEvent(window->m_width, window->m_height, window->GetBaseResource()));
				break;
			}

			return 0;
		}
		case WM_EXITSIZEMOVE:
		{
			BaseApplication::GetApplication()->PushEvent(new WindowResizedEvent(window->m_width, window->m_height, window->GetBaseResource()));
			return 0;
		}
		case WM_MOUSEMOVE:
		{
			if (window->m_cursor_enabled)
			{
				window->m_cursor_x = GET_X_LPARAM(l_param);
				window->m_cursor_y = GET_Y_LPARAM(l_param);

				if (!window->m_cursor_active) // can only detect cursor entered events
				{
					TRACKMOUSEEVENT tme = {};
					tme.cbSize = sizeof(TRACKMOUSEEVENT);
					tme.dwFlags = TME_LEAVE;
					tme.hwndTrack = window_handle;
					TrackMouseEvent(&tme);

					window->m_cursor_active = true;
					BaseApplication::GetApplication()->PushEvent(new CursorEnteredEvent(window->GetBaseResource()));
				}

				BaseApplication::GetApplication()->PushEvent(new CursorMovedEvent(window->m_cursor_x, window->m_cursor_y, window->GetBaseResource()));
			}

			return 0;
		}
		case WM_MOUSELEAVE:
		{
			if (window->m_cursor_active)
			{
				window->m_cursor_active = false;
				BaseApplication::GetApplication()->PushEvent(new CursorExitedEvent(window->GetBaseResource()));
			}

			return 0;
		}
		case WM_CLOSE:
			BaseApplication::GetApplication()->PushEvent(new AttemptWindowClosedEvent(window->GetBaseResource()));
			return 0;

		case WM_INPUT:
		{
			if (window->m_cursor_enabled) return 0;

			unsigned int size;
			HRAWINPUT hri = (HRAWINPUT)l_param;

			GetRawInputData(hri, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));

			if (size > window->m_raw_input_size)
			{
				window->m_raw_input_size = size;
				void *data = realloc(window->m_raw_input_data, size);
				
				if (!data)
				{
					free(window->m_raw_input_data);
					INFINITY_CORE_ERROR("Error reading virtual cursor position while cursor disabled");
					return 0;
				}

				window->m_raw_input_data = data;
			}
			
			if (GetRawInputData(hri, RID_INPUT, window->m_raw_input_data, &size, sizeof(RAWINPUTHEADER)) != size)
			{
				INFINITY_CORE_ERROR("Error reading virtual cursor position while cursor disabled");
				return 0;
			}

			RAWINPUT *data = (RAWINPUT*)window->m_raw_input_data;

			int cx = (int)data->data.mouse.lLastX;
			int cy = (int)data->data.mouse.lLastY;

			if (cx || cy)
			{
				window->m_cursor_x += cx;
				window->m_cursor_y += cy;

				BaseApplication::GetApplication()->PushEvent(new CursorMovedEvent(window->m_cursor_x, window->m_cursor_y, window->GetBaseResource()));
			}

			return 0;
		}
		default:
			return DefWindowProcA(window_handle, msg, w_param, l_param);
		}
	}
}

#endif // INFINITY_WINDOWS
