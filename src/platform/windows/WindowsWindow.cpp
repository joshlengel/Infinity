#include"InfinityPCH.h"

#ifdef INFINITY_WINDOWS

#include"platform/windows/WindowsWindow.h"

#include"platform/windows/WindowsContext.h"

#include"Application.h"
#include"event/Event.h"

constexpr static const char CLASS_NAME[] = "Infinity WindowsWindow Class";

static const Infinity::Map<unsigned int, Infinity::KeyCode> VK_TO_KEY_CODE =
{
	{ VK_BACK,       Infinity::KeyCode::Backspace     },
	{ VK_TAB,        Infinity::KeyCode::Tab           },
	{ VK_RETURN,     Infinity::KeyCode::Return        },
	{ VK_SHIFT,      Infinity::KeyCode::Left_Shift    },
	{ VK_LCONTROL,   Infinity::KeyCode::Left_Control  },
	{ VK_RCONTROL,   Infinity::KeyCode::Right_Control },
	{ VK_LMENU,      Infinity::KeyCode::Left_Menu     },
	{ VK_RMENU,      Infinity::KeyCode::Right_Menu    },
	{ VK_CAPITAL,    Infinity::KeyCode::Capslock      },
	{ VK_ESCAPE,     Infinity::KeyCode::Escape        },
	{ VK_SPACE,      Infinity::KeyCode::Space         },
	{ VK_END,        Infinity::KeyCode::End           },
	{ VK_HOME,       Infinity::KeyCode::Home          },
	{ VK_LEFT,       Infinity::KeyCode::Left          },
	{ VK_UP,         Infinity::KeyCode::Up            },
	{ VK_RIGHT,      Infinity::KeyCode::Right         }, 
	{ VK_DOWN,       Infinity::KeyCode::Down          },
	{ VK_INSERT,     Infinity::KeyCode::Insert        },
	{ VK_DELETE,     Infinity::KeyCode::Delete        },
	{ VK_MULTIPLY,   Infinity::KeyCode::Multiply      },
	{ VK_ADD,        Infinity::KeyCode::Add           },
	{ VK_SEPARATOR,  Infinity::KeyCode::Separator     },
	{ VK_SUBTRACT,   Infinity::KeyCode::Subtract      },
	{ VK_DECIMAL,    Infinity::KeyCode::Decimal       },
	{ VK_DIVIDE,     Infinity::KeyCode::Divide        },   
	{ VK_OEM_PLUS,   Infinity::KeyCode::Plus          },
	{ VK_OEM_COMMA,  Infinity::KeyCode::Comma         },
	{ VK_OEM_MINUS,  Infinity::KeyCode::Minus         },
	{ VK_OEM_PERIOD, Infinity::KeyCode::Period        },

	{ (unsigned int)'0', Infinity::KeyCode::Zero      },
	{ (unsigned int)'1', Infinity::KeyCode::One       },
	{ (unsigned int)'2', Infinity::KeyCode::Two       },
	{ (unsigned int)'3', Infinity::KeyCode::Three     },
	{ (unsigned int)'4', Infinity::KeyCode::Four      },
	{ (unsigned int)'5', Infinity::KeyCode::Five      },
	{ (unsigned int)'6', Infinity::KeyCode::Six       },
	{ (unsigned int)'7', Infinity::KeyCode::Seven     },
	{ (unsigned int)'8', Infinity::KeyCode::Eight     },
	{ (unsigned int)'9', Infinity::KeyCode::Nine      },

	{ (unsigned int)'A', Infinity::KeyCode::A         },
	{ (unsigned int)'B', Infinity::KeyCode::B         },
	{ (unsigned int)'C', Infinity::KeyCode::C         },
	{ (unsigned int)'D', Infinity::KeyCode::D         },
	{ (unsigned int)'E', Infinity::KeyCode::E         },
	{ (unsigned int)'F', Infinity::KeyCode::F         },
	{ (unsigned int)'G', Infinity::KeyCode::G         },
	{ (unsigned int)'H', Infinity::KeyCode::H         },
	{ (unsigned int)'I', Infinity::KeyCode::I         },
	{ (unsigned int)'J', Infinity::KeyCode::J         },
	{ (unsigned int)'K', Infinity::KeyCode::K         },
	{ (unsigned int)'L', Infinity::KeyCode::L         },
	{ (unsigned int)'M', Infinity::KeyCode::M         },
	{ (unsigned int)'N', Infinity::KeyCode::N         },
	{ (unsigned int)'O', Infinity::KeyCode::O         },
	{ (unsigned int)'P', Infinity::KeyCode::P         },
	{ (unsigned int)'Q', Infinity::KeyCode::Q         },
	{ (unsigned int)'R', Infinity::KeyCode::R         },
	{ (unsigned int)'S', Infinity::KeyCode::S         },
	{ (unsigned int)'T', Infinity::KeyCode::T         },
	{ (unsigned int)'U', Infinity::KeyCode::U         },
	{ (unsigned int)'V', Infinity::KeyCode::V         },
	{ (unsigned int)'W', Infinity::KeyCode::W         },
	{ (unsigned int)'X', Infinity::KeyCode::X         },
	{ (unsigned int)'Y', Infinity::KeyCode::Y         },
	{ (unsigned int)'Z', Infinity::KeyCode::Z         },

	{ VK_NUMPAD0, Infinity::KeyCode::NUMPAD_0         },
	{ VK_NUMPAD1, Infinity::KeyCode::NUMPAD_1         },
	{ VK_NUMPAD2, Infinity::KeyCode::NUMPAD_2         },
	{ VK_NUMPAD3, Infinity::KeyCode::NUMPAD_3         },
	{ VK_NUMPAD4, Infinity::KeyCode::NUMPAD_4         },
	{ VK_NUMPAD5, Infinity::KeyCode::NUMPAD_5         },
	{ VK_NUMPAD6, Infinity::KeyCode::NUMPAD_6         },
	{ VK_NUMPAD7, Infinity::KeyCode::NUMPAD_7         },
	{ VK_NUMPAD8, Infinity::KeyCode::NUMPAD_8         },
	{ VK_NUMPAD9, Infinity::KeyCode::NUMPAD_9         },
	
	{ VK_F1,  Infinity::KeyCode::F1                    },
	{ VK_F2,  Infinity::KeyCode::F2                    },
	{ VK_F3,  Infinity::KeyCode::F3                    },
	{ VK_F4,  Infinity::KeyCode::F4                    },
	{ VK_F5,  Infinity::KeyCode::F5                    },
	{ VK_F6,  Infinity::KeyCode::F6                    },
	{ VK_F7,  Infinity::KeyCode::F7                    },
	{ VK_F8,  Infinity::KeyCode::F8                    },
	{ VK_F9,  Infinity::KeyCode::F9                    },
	{ VK_F10, Infinity::KeyCode::F10                   },
	{ VK_F11, Infinity::KeyCode::F11                   },
	{ VK_F12, Infinity::KeyCode::F12                   },
	{ VK_F13, Infinity::KeyCode::F13                   },
	{ VK_F14, Infinity::KeyCode::F14                   },
	{ VK_F15, Infinity::KeyCode::F15                   },
	{ VK_F16, Infinity::KeyCode::F16                   },
	{ VK_F17, Infinity::KeyCode::F17                   },
	{ VK_F18, Infinity::KeyCode::F18                   },
	{ VK_F19, Infinity::KeyCode::F19                   },
	{ VK_F20, Infinity::KeyCode::F20                   },
	{ VK_F21, Infinity::KeyCode::F21                   },
	{ VK_F22, Infinity::KeyCode::F22                   },
	{ VK_F23, Infinity::KeyCode::F23                   },
	{ VK_F24, Infinity::KeyCode::F24                   }
};

static const Infinity::Map<unsigned int, Infinity::MouseCode> VK_TO_MOUSE_CODE =
{
	{ VK_LBUTTON, Infinity::MouseCode::Left },
	{ VK_RBUTTON, Infinity::MouseCode::Right },
	{ VK_MBUTTON, Infinity::MouseCode::Middle }
};

static const int FULLSCREEN_HOTKEY_ID = GlobalAddAtomA("FullscreenHotkeyID");

constexpr static const long WINDOWED_STYLE = WS_OVERLAPPEDWINDOW;
constexpr static const long WINDOWED_STYLE_EX = WS_EX_APPWINDOW;
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
	bool Window::Init()
	{
		window_instance = GetModuleHandle(nullptr);

		if (!window_instance) return false;

		WNDCLASSA wc = {};
		wc.hInstance = window_instance;
		wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
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

		std::unique_ptr<DXGI_MODE_DESC[]> modes = std::make_unique<DXGI_MODE_DESC[]>(num_modes);
		if (FAILED(output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &num_modes, modes.get()))) return false;

		for (unsigned int i = 0; i < num_modes; ++i)
		{
			const DXGI_MODE_DESC &mode = modes[i];

			if (mode.Width == screen_width && mode.Height == screen_height)
			{
				refresh_rate_numerator = mode.RefreshRate.Numerator;
				refresh_rate_denominator = mode.RefreshRate.Denominator;
			}
		}

		output->Release();
		adapter->Release();
		factory->Release();

		Application::GetApplication()->AddEventListener(WindowsWindow::EventListener);

		return true;
	}

	Window *Window::CreateWindow()
	{
		return new WindowsWindow;
	}

	bool WindowsWindow::hotkey_registered = false;
	bool WindowsWindow::main_window_fullscreened = false;
	bool WindowsWindow::main_window_alt_enter_enabled;

	bool WindowsWindow::keys[];
	bool WindowsWindow::buttons[];

	WindowsWindow::WindowsWindow():
		m_window_handle(nullptr),
		m_should_close(false),
		m_restored_state(),
		m_style(),
		m_style_ex(),

		m_width(),
		m_height(),
		m_prev_width(),
		m_prev_height(),

		m_vsync(),
		m_fullscreen(),

		m_swap_chain(nullptr),
		m_device(nullptr),
		m_device_context(nullptr),
		m_render_target_view(nullptr),
		m_depth_stencil_buffer(nullptr),
		m_depth_stencil_view(nullptr),
		m_depth_stencil_state(nullptr),

		m_cursor_enabled(true),
		m_cursor_x(),
		m_cursor_y(),
		m_restore_cursor_x(),
		m_restore_cursor_y(),

		m_cursor_active(),
		m_raw_input_size(0),
		m_raw_input_data(nullptr),

		m_showing(false),
		m_is_main_window(false)
	{}

	WindowsWindow::~WindowsWindow()
	{
		if (m_raw_input_data)
		{
			free(m_raw_input_data);
		}
	}

	bool WindowsWindow::Init(const WindowParams &params)
	{
		m_vsync = params.vsync;
		m_fullscreen = Window::main_window? false : params.fullscreen;

		m_auto_swap_buffers = params.auto_swap_buffers;
		
		m_style = WINDOWED_STYLE;
		m_style_ex = WINDOWED_STYLE_EX;

		int x = params.x == INFINITY_DONT_CARE? CW_USEDEFAULT : (signed)params.x;
		int y = params.y == INFINITY_DONT_CARE? CW_USEDEFAULT : (signed)params.y;
		int width = params.width == INFINITY_DONT_CARE? CW_USEDEFAULT : (signed)params.width;
		int height = params.height == INFINITY_DONT_CARE? CW_USEDEFAULT : (signed)params.height;

		HWND parent_window = nullptr;

		if (Window::main_window)
		{
			parent_window = ((WindowsWindow*)Window::main_window)->m_window_handle;
		}

		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = width;
		rect.bottom = height;

		AdjustWindowRectEx(&rect, m_style, false, m_style_ex);

		m_window_handle = CreateWindowExA(m_style_ex, CLASS_NAME, params.title, m_style,
			x,
			y,
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

		unsigned int a_x =      (unsigned int) window_rect.left;
		unsigned int a_y =      (unsigned int) window_rect.top;
		unsigned int a_width =  (unsigned int)(window_rect.right - window_rect.left);
		unsigned int a_height = (unsigned int)(window_rect.bottom - window_rect.top);

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
		if (FAILED(m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&device_dxgi))) return false;

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

		if (!hotkey_registered)
		{
			m_is_main_window = true;

			Window::main_window = this;

			main_window_fullscreened = m_fullscreen;
			main_window_alt_enter_enabled = params.enable_alt_enter_fullscreen;

			if (!RegisterHotKey(m_window_handle, FULLSCREEN_HOTKEY_ID, MOD_ALT | MOD_NOREPEAT, VK_RETURN))
			{
				INFINITY_CORE_WARN("Error setting fullscreen hotkey. Mode switch may not be supported");
			}

			hotkey_registered = true;
		}

		m_context = new WindowsContext(m_device, m_device_context);
		
		Context *restore_con = context;
		context = m_context; // Context.Init creates a rasterizer so current context must be bound beforehand
		if (!m_context->Init())
		{
			INFINITY_CORE_ERROR("Error initializing Context for window");
			return false;
		}
		context = restore_con;

		if (m_fullscreen)
		{
			Resize();
			EnterFullscreenMode();
		}

		Application::GetApplication()->PushEvent(new WindowResizedEvent(this, m_width, m_height, this));
		Application::GetApplication()->AddWindow(this);
		
		return true;
	}

	void WindowsWindow::Destroy()
	{
		if (Window::context == m_context) Window::context = nullptr;

		if (m_context)
		{
			m_context->Destroy();
			delete m_context;
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

		if (m_depth_stencil_state)
		{
			m_depth_stencil_state->Release();
			m_depth_stencil_state = nullptr;
		}

		if (m_depth_stencil_view)
		{
			m_depth_stencil_view->Release();
			m_depth_stencil_view = nullptr;
		}

		if (m_swap_chain)
		{
			m_swap_chain->SetFullscreenState(false, nullptr);

			m_swap_chain->Release();
			m_swap_chain = nullptr;
		}

		if (m_device)
		{
			m_device->Release();
			m_device = nullptr;
		}

		if (m_device_context)
		{
			m_device_context->Release();
			m_device_context = nullptr;
		}

		Application::GetApplication()->RemoveWindow(this);
	}

	void WindowsWindow::Show()
	{
		if (m_is_main_window || !main_window_fullscreened)
		{
			ShowWindow(m_window_handle, SW_SHOW);
		}

		m_showing = true;
	}

	void WindowsWindow::Hide()
	{
		if (m_is_main_window || !main_window_fullscreened)
		{
			ShowWindow(m_window_handle, SW_HIDE);
		}

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

	void Window::PollInput()
	{
		MSG msg;

		while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) return;

			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}

		for (auto &entry : VK_TO_KEY_CODE)
		{
			bool key_pressed = GetAsyncKeyState(entry.key) & 0x8000;

			if (WindowsWindow::keys[(unsigned int)entry.value])
			{
				if (!key_pressed)
				{
					WindowsWindow::keys[(unsigned int)entry.value] = false;
					Application::GetApplication()->PushEvent(new KeyReleasedEvent(entry.value, nullptr));
				}
			}
			else
			{
				if (key_pressed)
				{
					WindowsWindow::keys[(unsigned int)entry.value] = true;
					Application::GetApplication()->PushEvent(new KeyPressedEvent(entry.value, nullptr));
				}
			}
		}

		for (auto &entry : VK_TO_MOUSE_CODE)
		{
			bool key_pressed = GetAsyncKeyState(entry.key) & 0x8000;

			if (WindowsWindow::buttons[(unsigned int)entry.value])
			{
				if (!key_pressed)
				{
					WindowsWindow::buttons[(unsigned int)entry.value] = false;
					Application::GetApplication()->PushEvent(new MouseReleasedEvent(entry.value, nullptr));
				}
			}
			else
			{
				if (key_pressed)
				{
					WindowsWindow::buttons[(unsigned int)entry.value] = true;
					Application::GetApplication()->PushEvent(new MousePressedEvent(entry.value, nullptr));
				}
			}
		}
	}

	void WindowsWindow::SwapBuffers()
	{
		if (m_vsync) m_swap_chain->Present(1, 0);
		else	     m_swap_chain->Present(0, 0);
	}

	bool WindowsWindow::KeyDown(KeyCode key) const
	{
		for (auto &entry : VK_TO_KEY_CODE)
		{
			if (entry.value == key) return GetAsyncKeyState(entry.key) & 0x8000; // entry.key has nothing to do with KeyCode key
		}

		return false;
	}

	bool WindowsWindow::KeyPressed(KeyCode key) const { return KeyDown(key) && !keys[(unsigned int)key]; }
	bool WindowsWindow::KeyReleased(KeyCode key) const { return !KeyDown(key) && keys[(unsigned int)key]; }
	
	bool WindowsWindow::MouseDown(MouseCode button) const
	{
		for (auto &entry : VK_TO_MOUSE_CODE)
		{
			if (entry.value == button) return GetAsyncKeyState(entry.key) & 0x8000;
		}

		return false;
	}

	bool WindowsWindow::MousePressed(MouseCode button) const { return MouseDown(button) && !buttons[(unsigned int)button]; }
	bool WindowsWindow::MouseReleased(MouseCode button) const { return !MouseDown(button) && buttons[(unsigned int)button]; }

	void WindowsWindow::UpdateClipRect()
	{
		if (CursorEnabled())
		{
			ClipCursor(nullptr);
		}
		else
		{
			RECT client_rect;
			GetClientRect(m_window_handle, &client_rect);
			ClientToScreen(m_window_handle, (POINT*)&client_rect.left);
			ClientToScreen(m_window_handle, (POINT*)&client_rect.right);
			ClipCursor(&client_rect);
		}
	}

	void WindowsWindow::EnableCursor()
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
	}

	void WindowsWindow::DisableCursor()
	{
		m_cursor_enabled = false;

		m_restore_cursor_x = m_cursor_x;
		m_restore_cursor_y = m_cursor_y;

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
	}

	bool WindowsWindow::CursorEnabled() const { return m_cursor_enabled; }

	int WindowsWindow::GetCursorPosX() const { return m_cursor_x; }
	int WindowsWindow::GetCursorPosY() const { return m_cursor_y; }

	bool WindowsWindow::Resize()
	{
		if (m_width != m_prev_width || m_height != m_prev_height)
		{
			m_prev_width = m_width;
			m_prev_height = m_height;

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
				Application::GetApplication()->RequestExit();
				return false;
			}
			
			ID3D11Texture2D *back_buffer;
			if (FAILED(m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer)))
			{
				INFINITY_CORE_ERROR("Error getting window buffer after resize");
				Application::GetApplication()->RequestExit();
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
				Application::GetApplication()->RequestExit();
				return false;
			}

			back_buffer->Release();

			D3D11_TEXTURE2D_DESC dsb_desc = back_buffer_desc;
			dsb_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			dsb_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

			if (FAILED(m_device->CreateTexture2D(&dsb_desc, nullptr, &m_depth_stencil_buffer)))
			{
				INFINITY_CORE_ERROR("Error creating depth stencil buffer after resizing context");
				Application::GetApplication()->RequestExit();
				return false;
			}

			D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};
			dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
			dsv_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

			if (FAILED(m_device->CreateDepthStencilView(m_depth_stencil_buffer, &dsv_desc, &m_depth_stencil_view)))
			{
				INFINITY_CORE_ERROR("Error creating depth stencil view after resizing context");
				Application::GetApplication()->RequestExit();
				return false;
			}
			
			WindowsContext *win_con = (WindowsContext*)m_context;

			if (win_con)
			{
				if (!win_con->Resize(m_render_target_view, m_depth_stencil_view, m_width, m_height))
				{
					INFINITY_CORE_ERROR("Error resizing context");
					Application::GetApplication()->RequestExit();
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

	void WindowsWindow::EventListener(Event *event)
	{
		switch (event->GetType())
		{
		case Event::EventType::WindowResized:
		{
			WindowsWindow *window = (WindowsWindow*)event->GetCaller();

			if (!window->Resize())
				event->Consume();

			break;
		}
		case Event::EventType::WindowClosed:
		{
			WindowsWindow *window = (WindowsWindow*)event->GetCaller();
			window->m_should_close = true;
			Application::GetApplication()->RemoveWindow(window);
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
			Application::GetApplication()->RequestExit();
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
			Application::GetApplication()->RequestExit();
			return;
		}

		SetWindowLongA(m_window_handle, GWL_STYLE, WINDOWED_STYLE);
		SetWindowLongA(m_window_handle, GWL_EXSTYLE, WINDOWED_STYLE_EX);

		if (!SetWindowPos(m_window_handle, nullptr, m_restored_state.left, m_restored_state.top,
			m_restored_state.right, m_restored_state.bottom, SWP_FRAMECHANGED | SWP_NOZORDER))
		{
			INFINITY_CORE_ERROR("Error resizing window after exiting fullscreen mode");
			Application::GetApplication()->RequestExit();
			return;
		}

		UpdateClipRect();

		GetClientRect(m_window_handle, &m_restored_state);

		m_width = m_restored_state.right;
		m_height = m_restored_state.bottom;

		ShowWindow(m_window_handle, m_showing? SW_SHOW : SW_HIDE);
	}

	LRESULT WindowsWindow::WindowProcedure(HWND window_handle, UINT msg, WPARAM w_param, LPARAM l_param)
	{
		WindowsWindow *window = reinterpret_cast<WindowsWindow*>(GetWindowLongPtrA(window_handle, GWLP_USERDATA));

		switch (msg)
		{
		case WM_WINDOWPOSCHANGED:
		{
			RECT rect;
			GetClientRect(window_handle, &rect);

			window->m_width = (unsigned)rect.right;
			window->m_height = (unsigned)rect.bottom;

			return 0;
		}
		case WM_HOTKEY:
			if (w_param == (unsigned)FULLSCREEN_HOTKEY_ID && main_window_alt_enter_enabled)
			{
				window->m_fullscreen = !window->m_fullscreen;
				
				if (window->m_fullscreen)
				{
					main_window_fullscreened = true;
					
					for (Window *window : Application::GetApplication()->GetWindows())
					{
						ShowWindow(((WindowsWindow*)window)->m_window_handle, SW_HIDE);
					}

					window->EnterFullscreenMode();

					Application::GetApplication()->PushEvent(new WindowResizedEvent(window, window->m_width, window->m_height, window));
				}
				else
				{
					main_window_fullscreened = false;

					window->ExitFullscreenMode();

					for (Window *window : Application::GetApplication()->GetWindows())
					{
						WindowsWindow *wwin = (WindowsWindow*)window;

						if (wwin->m_showing)
						{
							ShowWindow(wwin->m_window_handle, SW_SHOW);
						}
					}

					Application::GetApplication()->PushEvent(new WindowResizedEvent(window, window->m_width, window->m_height, window));
				}
			}

			return 0;
		case WM_EXITSIZEMOVE:
		{
			Application::GetApplication()->PushEvent(new WindowResizedEvent(window, window->m_width, window->m_height, window));
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
					Application::GetApplication()->PushEvent(new CursorEnteredEvent(window));
				}

				Application::GetApplication()->PushEvent(new CursorMovedEvent(window->m_cursor_x, window->m_cursor_y, window));
			}

			return 0;
		}
		case WM_MOUSELEAVE:
		{
			if (window->m_cursor_active)
			{
				window->m_cursor_active = false;
				Application::GetApplication()->PushEvent(new CursorExitedEvent(window));
			}

			return 0;
		}
		case WM_CLOSE:
			Application::GetApplication()->PushEvent(new WindowClosedEvent(window, window));
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

				Application::GetApplication()->PushEvent(new CursorMovedEvent(window->m_cursor_x, window->m_cursor_y, window));
			}

			return 0;
		}
		default:
			return DefWindowProcA(window_handle, msg, w_param, l_param);
		}
	}
}

#endif // INFINITY_WINDOWS
