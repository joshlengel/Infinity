#pragma once

#include"Core.h"

#include"Memory.h"

#include<initializer_list>

namespace Infinity
{
	template <typename T>
	class INFINITY_API ArrayList
	{
	private:
		constexpr static const unsigned int STACK_DATA_LENGTH = 10;

		unsigned int m_size, m_capacity, m_prev_capacity;
		T *m_data;
		T m_stack_data[STACK_DATA_LENGTH];

	public:
		ArrayList(unsigned int capacity = STACK_DATA_LENGTH):
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

		ArrayList(const T *elems, unsigned int size):
			m_size(size),
			m_capacity(),
			m_prev_capacity(),
			m_data(nullptr),
			m_stack_data()
		{
			if (size > STACK_DATA_LENGTH)
			{
				m_capacity = size;
				m_prev_capacity = size;

				m_data = new T[m_size];
				Copy(elems, m_data, m_size);
			}
			else
			{
				m_capacity = STACK_DATA_LENGTH;
				m_prev_capacity = STACK_DATA_LENGTH;

				m_data = m_stack_data;
				Copy(elems, m_stack_data, m_size);
			}
		}

		ArrayList(std::initializer_list<T> list):
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

		ArrayList(const ArrayList &list):
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

		ArrayList(ArrayList &&list) noexcept:
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

		~ArrayList()
		{
			if (m_data && m_capacity > STACK_DATA_LENGTH) delete[] m_data;
		}

		ArrayList &operator=(const ArrayList &list)
		{
			if (list.m_size <= m_size)
			{
				m_size = list.m_size;
				Copy(list.m_data, m_data, list.m_size);

				ShrinkCapacity();
			}
			else
			{
				if (m_capacity > STACK_DATA_LENGTH)
					delete[] m_data;

				m_size = list.m_size;
				m_capacity = list.m_capacity;
				m_prev_capacity = list.m_prev_capacity;

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

			return *this;
		}

		ArrayList &operator=(ArrayList &&list) noexcept
		{
			if (m_capacity > STACK_DATA_LENGTH)
				delete[] m_data;

			m_data = list.m_data;

			if (list.m_capacity <= STACK_DATA_LENGTH)
			{
				Copy(m_data, m_stack_data, list.m_size);
				m_data = m_stack_data;
			}

			m_size = list.m_size;
			m_capacity = list.m_capacity;
			m_prev_capacity = list.m_prev_capacity;

			list.m_size = 0;
			list.m_capacity = 0;
			list.m_prev_capacity = 0;
			list.m_data = nullptr;

			return *this;
		}

		const T &operator[](unsigned int i) const { return m_data[i]; }
		T &operator[](unsigned int i) { return m_data[i]; }

		void Add(const T &elem)
		{
			EnsureCapacity();

			m_data[m_size++] = elem;
		}

		void Add(const T *elems, unsigned int num)
		{
			EnsureCapacity(num);

			Copy(elems, m_data, num, m_size, 0);
		}

		void Add(const ArrayList &list)
		{
			Add(list.m_data, list.m_size);
		}

		void Remove(unsigned int i)
		{
			Copy(m_data, m_data, m_size - i - 1, i + 1, i); // In-place copy. This only works under very specific circumstances.

			--m_size;

			ShrinkCapacity();
		}

		void RemoveBack()
		{
			--m_size;

			ShrinkCapacity();
		}

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

		const T *begin() const { return m_data; }
		const T *end() const { return m_data + m_size; }
		T *begin() { return m_data; }
		T *end() { return m_data + m_size; }

	private:
		void EnsureCapacity(unsigned int num = 1)
		{
			if (m_size + num > m_capacity)
			{
				while (m_size + num > m_capacity)
				{
					m_prev_capacity = m_capacity;
					m_capacity += m_capacity;
				}

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