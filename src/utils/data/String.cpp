#include"InfinityPCH.h"

#include"utils/data/String.h"

namespace Infinity
{
	bool StaticString::operator==(const StaticString &str) const
	{
		if (m_length != str.m_length) return false;

		const char *itr1 = m_string;
		const char *itr2 = str.m_string;

		while (*itr1) if (*itr1++ != *itr2++) return false;

		return true;
	}

	const char &StaticString::operator[](unsigned int i) const
	{
		return m_string[i];
	}
}