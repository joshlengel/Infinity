#include"InfinityPCH.h"

#ifdef INFINITY_WINDOWS

#include"event/Event.h"
#include"event/InputCodes.h"

#include"Application.h"

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

namespace Infinity
{
	bool KeyDown(KeyCode key)
	{
		for (auto &entry : VK_TO_KEY_CODE)
		{
			if (entry.value == key) return GetAsyncKeyState(entry.key) & 0x8000; // entry.key and key don't mean the same thing
		}

		return false;
	}

	bool KeyPressed(KeyCode key) { return KeyDown(key) && !_Impl::keys[(unsigned int)key]; }
	bool KeyReleased(KeyCode key) { return !KeyDown(key) && _Impl::keys[(unsigned int)key]; }

	bool MouseDown(MouseCode button)
	{
		for (auto &entry : VK_TO_MOUSE_CODE)
		{
			if (entry.value == button) return GetAsyncKeyState(entry.key) & 0x8000;
		}

		return false;
	}

	bool MousePressed(MouseCode button) { return MouseDown(button) && !_Impl::buttons[(unsigned int)button]; }
	bool MouseReleased(MouseCode button) { return !MouseDown(button) && _Impl::buttons[(unsigned int)button]; }

	namespace _Impl
	{
		bool keys[(unsigned int)KeyCode::Last];
		bool buttons[(unsigned int)MouseCode::Last];

		void Update()
		{
			for (auto &entry : VK_TO_KEY_CODE)
			{
				bool key_pressed = GetAsyncKeyState(entry.key) & 0x8000;

				if (keys[(unsigned int)entry.value])
				{
					if (!key_pressed)
					{
						keys[(unsigned int)entry.value] = false;
						Application::GetApplication()->PushEvent(new KeyReleasedEvent(entry.value, nullptr));
					}
				}
				else
				{
					if (key_pressed)
					{
						keys[(unsigned int)entry.value] = true;
						Application::GetApplication()->PushEvent(new KeyPressedEvent(entry.value, nullptr));
					}
				}
			}

			for (auto &entry : VK_TO_MOUSE_CODE)
			{
				bool key_pressed = GetAsyncKeyState(entry.key) & 0x8000;

				if (buttons[(unsigned int)entry.value])
				{
					if (!key_pressed)
					{
						buttons[(unsigned int)entry.value] = false;
						Application::GetApplication()->PushEvent(new MouseReleasedEvent(entry.value, nullptr));
					}
				}
				else
				{
					if (key_pressed)
					{
						buttons[(unsigned int)entry.value] = true;
						Application::GetApplication()->PushEvent(new MousePressedEvent(entry.value, nullptr));
					}
				}
			}
		}
	}
}

#endif // INFINITY_WINDOW