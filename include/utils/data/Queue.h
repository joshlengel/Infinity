#pragma once

#include"Core.h"

#include"Memory.h"

#include<initializer_list>

namespace Infinity
{
	template <typename T>
	class INFINITY_API Queue
	{
	private:
		constexpr static const unsigned int STACK_DATA_LENGTH = 10;

		unsigned int m_size, m_capacity, m_prev_capacity;
		T *m_data;
		T m_stack_data[STACK_DATA_LENGTH];

	public:
		Queue(unsigned int capacity = STACK_DATA_LENGTH):
			m_size(0),
			m_capacity(capacity),
			m_prev_capacity(capacity),
			m_data(nullptr),
			m_stack_data()
		{
			if (capacity > STACK_DATA_LENGTH)
			{
				m_data = new T[capacity];
			}
			else
			{
				m_data = m_stack_data;
			}
		}

		Queue(std::initializer_list<T> list):
			m_size((unsigned int)list.size()),
			m_capacity((unsigned int)list.size()),
			m_prev_capacity((unsigned int)list.size()),
			m_data(nullptr),
			m_stack_data()
		{
			if (m_capacity > STACK_DATA_LENGTH)
			{
				m_data = new T[m_capacity];
			}
			else
			{
				m_data = m_stack_data;
			}

			Copy(m_data, list.begin(), m_size);
		}

		Queue(const Queue &list):
			m_size(list.m_size),
			m_capacity(list.m_capacity),
			m_prev_capacity(list.m_prev_capacity),
			m_data(nullptr),
			m_stack_data()
		{
			if (m_capacity > STACK_DATA_LENGTH)
			{
				m_data = new T[m_capacity];
			}
			else
			{
				m_data = m_stack_data;
			}

			Copy(m_data, list.m_data, m_size);
		}

		Queue(Queue &&list) noexcept:
			m_size(list.m_size),
			m_capacity(list.m_capacity),
			m_prev_capacity(list.m_prev_capacity),
			m_data(list.m_data),
			m_stack_data()
		{
			list.m_size = 0;
			list.m_capacity = 0;
			list.m_prev_capacity = 0;
			list.m_data = nullptr;

			if (m_capacity <= STACK_DATA_LENGTH)
			{
				Move(m_stack_data, m_data, m_size);
				m_data = m_stack_data;
			}
		}

		~Queue()
		{
			if (m_data && m_capacity > STACK_DATA_LENGTH) delete[] m_data;
		}

		const T &operator[](unsigned int i) const { return m_data[i]; }
		T &operator[](unsigned int i) { return m_data[i]; }

		void Push(const T &elem)
		{
			EnsureCapacity();

			m_data[m_size++] = elem;
		}

		void Pop()
		{
			--m_size;
			
			T *itr1 = m_data;
			T *itr2 = m_data + 1;
			
			for (unsigned int i = 0; i < m_size; ++i)
			{
				*itr1++ = std::move(*itr2++);
			}

			ShrinkCapacity();
		}

		const T &Peek() const { return m_data[0]; }
		T &Peek() { return m_data[0]; }

		void Clear()
		{
			m_size = 0;

			if (m_capacity > STACK_DATA_LENGTH)
				delete[] m_data;

			m_data = m_stack_data;

			m_capacity = STACK_DATA_LENGTH;
			m_prev_capacity = STACK_DATA_LENGTH;
		}

		unsigned int GetSize() const { return m_size; }
		unsigned int GetCapacity() const { return m_capacity; }

		bool Empty() const { return !m_size; }

	private:
		void EnsureCapacity()
		{
			if (m_size == m_capacity)
			{
				m_prev_capacity = m_capacity;
				m_capacity += m_capacity;

				if (m_capacity > STACK_DATA_LENGTH)
				{
					T *temp = new T[m_capacity];
					Move(temp, m_data, m_prev_capacity);

					if (m_prev_capacity > STACK_DATA_LENGTH)
						delete[] m_data;

					m_data = temp;
				}
			}
		}

		void ShrinkCapacity()
		{
			if (m_size < m_prev_capacity)
			{
				if (m_size <= STACK_DATA_LENGTH)
				{
					if (m_prev_capacity > STACK_DATA_LENGTH)
					{
						Move(m_stack_data, m_data, m_size);

						delete[] m_data;

						m_data = m_stack_data;
					}
				}
				else
				{
					T *temp = new T[m_prev_capacity];
					Move(temp, m_data, m_size);

					delete[] m_data;

					m_data = temp;
				}

				m_capacity = m_prev_capacity;
				m_prev_capacity = m_size / 2;
			}
		}

		static void Copy(T *dest, const T *src, unsigned int elems)
		{
			const T *itr1 = src;
			T *itr2 = dest;

			for (unsigned int i = 0; i < elems; ++i)
			{
				*itr2++ = *itr1++;
			}
		}

		static void Move(T *dest, T *src, unsigned int elems)
		{
			T *itr1 = src;
			T *itr2 = dest;

			for (unsigned int i = 0; i < elems; ++i)
			{
				*itr2++ = std::move(*itr1++);
			}
		}
	};
}