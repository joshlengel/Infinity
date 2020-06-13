#pragma once

#include"Core.h"

#include<memory>

namespace Infinity
{
	template <typename T>
	class INFINITY_API Pointer
	{
	private:
		std::shared_ptr<T> *m_p;

	public:
		Pointer(std::shared_ptr<T> *p):
			m_p(p)
		{}

		Pointer():
			m_p(nullptr)
		{}

		Pointer(const Pointer &p):
			m_p(p.m_p)
		{}

		Pointer &operator=(const Pointer &p)
		{
			m_p = p.m_p;
			return *this;
		}

		T *operator->() const { return m_p->operator->(); }
		T &operator*() const { return m_p->operator*(); }
	};
}