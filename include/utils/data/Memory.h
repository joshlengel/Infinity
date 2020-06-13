#pragma once

#include"Core.h"

namespace Infinity
{
	template <typename T>
	static void Copy(const T *source, T *dest, unsigned int elems, unsigned int source_start = 0, unsigned int dest_start = 0)
	{
		T *d = static_cast<T*>(dest + dest_start);
		const T *c = static_cast<const T*>(source + source_start);

		for (unsigned int i = 0; i < elems; ++i)
		{
			*d = *c;

			++d;
			++c;
		}
	}

	template <typename T>
	static void Set(T *dest, T value, unsigned int elems)
	{
		T *d = static_cast<T*>(dest);

		for (unsigned int i = 0; i < elems; ++i)
		{
			*d = value;

			++d;
		}
	}
}