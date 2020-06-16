#pragma once

#include"Core.h"

#include<memory>

namespace Infinity
{
	template <typename T>
	class INFINITY_API Pointer
	{
	private:
		struct Control
		{
			T val;
			unsigned int ref_count;
		};

		Control *m_control;

		Pointer(Control *control):
			m_control(control)
		{}

	public:
		inline static unsigned int ref_count = 0;

		template <typename ...Args>
		static Pointer Make(const Args &...args)
		{
			Control *control = new Control{ T(args...), 1 };

			++ref_count;

			return Pointer(control);
		}

		Pointer(const Pointer &p):
			m_control(p.m_control)
		{
			if (m_control)
				++m_control->ref_count;
		}

		Pointer(Pointer &&p):
			m_control(p.m_control)
		{
			p.m_control = nullptr;
		}

		Pointer():
			m_control(nullptr)
		{}

		~Pointer()
		{
			if (m_control)
			{
				--m_control->ref_count;

				if (m_control->ref_count == 0)
				{
					--ref_count;
					delete m_control;
				}
			}
		}

		Pointer &operator=(const Pointer &p)
		{
			if (m_control)
			{
				--m_control->ref_count;

				if (m_control->ref_count == 0)
				{
					--ref_count;
					delete m_control;
				}
			}

			m_control = p.m_control;
			
			if (m_control)
				++m_control->ref_count;
			
			return *this;
		}

		Pointer &operator=(Pointer &&p)
		{
			if (m_control)
			{
				--m_control->ref_count;

				if (m_control->ref_count == 0)
				{
					--ref_count;
					delete m_control;
				}
			}

			m_control = p.m_control;
			p.m_control = nullptr;
			return *this;
		}

		T *operator->() const { return &m_control->val; }
		T &operator*() const { return m_control->val; }

		operator bool()
		{
			return m_control;
		}
	};
}