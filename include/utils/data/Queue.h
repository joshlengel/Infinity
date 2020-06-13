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

			Copy(list.begin(), m_data, m_size);
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

			Copy(list.m_data, m_data, m_size);
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
				Copy(m_data, m_stack_data, m_size);
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
			Copy(m_data, m_data, m_size, 1, 0); // in-place copy. This only works due to the design of Copy()
			
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
					Copy(m_data, temp, m_prev_capacity);

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
						Copy(m_data, m_stack_data, m_size);

						delete[] m_data;

						m_data = m_stack_data;
					}
				}
				else
				{
					T *temp = new T[m_prev_capacity];
					Copy(m_data, temp, m_size);

					delete[] m_data;

					m_data = temp;
				}

				m_capacity = m_prev_capacity;
				m_prev_capacity = m_size / 2;
			}
		}
	};
}