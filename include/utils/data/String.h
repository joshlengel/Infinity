#pragma once

#include"Core.h"
#include"ArrayList.h"

#include<utility>

namespace Infinity
{
	INFINITY_TEMPLATE template class INFINITY_API ArrayList<char>;

	class INFINITY_API String
	{
	private:
		ArrayList<char> m_chars;

		static unsigned int Length(const char *str)
		{
			unsigned int size = 0;
			const char *c = str;

			while (*c != '\0')
			{
				++size;
				++c;
			}

			return size;
		}

	public:
		String():
			m_chars(1)
		{
			m_chars.Add('\0');
		}

		template <unsigned int Length>
		String(const char (&str)[Length]):
			m_chars(str, Length)
		{}

		String(const char *str):
			m_chars(str, Length(str) + 1)
		{}

		String(const String &str):
			m_chars(str.m_chars)
		{}

		String(String &&str) noexcept:
			m_chars(std::forward<ArrayList<char>>(str.m_chars))
		{}

		String &operator=(const String &str)
		{
			m_chars = str.m_chars;
			return *this;
		}

		String &operator=(String &&str) noexcept
		{
			m_chars = std::forward<ArrayList<char>>(str.m_chars);
			return *this;
		}

		unsigned int Length() const { return m_chars.GetSize() - 1; }

		char &operator[](unsigned int i) { return m_chars[i]; }
		const char &operator[](unsigned int i) const { return m_chars[i]; }

		bool operator==(const String &str) const
		{
			if (m_chars.GetSize() != str.m_chars.GetSize()) return false;

			const char *itr1 = m_chars.begin();
			const char *itr2 = str.m_chars.begin();

			while (itr1 != m_chars.end())
			{
				if (*itr1 != *itr2) return false;

				++itr1;
				++itr2;
			}

			return true;
		}
	};
}