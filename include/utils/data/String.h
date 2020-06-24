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
			m_chars()
		{}

		String(const char *str):
			m_chars(str, Length(str) + 1)
		{}

		String(const String &str):
			m_chars(str.m_chars)
		{}

		String(String &&str) noexcept:
			m_chars(std::move(str.m_chars))
		{}

		String &operator=(const String &str)
		{
			m_chars = str.m_chars;
			return *this;
		}

		String &operator=(String &&str) noexcept
		{
			m_chars = std::move(str.m_chars);
			return *this;
		}

		unsigned int Length() const { return m_chars.GetSize(); }

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

	class INFINITY_API StaticString
	{
	private:
		unsigned int m_length;
		const char *m_string;

	public:
		StaticString(): m_length(0), m_string(nullptr) {}

		template <unsigned int Length>
		StaticString(const char (&str)[Length]):
			m_length(Length),
			m_string(str)
		{}

		bool operator==(const StaticString &str) const;

		const char &operator[](unsigned int i) const;

		unsigned int GetLength() const { return m_length; }
		const char *Get() const { return m_string; }
	};
}