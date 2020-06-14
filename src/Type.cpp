#include"InfinityPCH.h"

#include"Type.h"

constexpr static const unsigned int BYTE_NUMS[] =
{
	sizeof(int),
	sizeof(int) * 2,
	sizeof(int) * 3,
	sizeof(int) * 4,
	sizeof(float),
	sizeof(float) * 2,
	sizeof(float) * 3,
	sizeof(float) * 4,
	sizeof(double),
	sizeof(double) * 2,
	sizeof(double) * 3,
	sizeof(double) * 4,
	sizeof(float) * 9,
	sizeof(float) * 16
};

constexpr static const unsigned int COUNTS[] =
{
	1,
	2,
	3,
	4,
	1,
	2,
	3,
	4,
	1,
	2,
	3,
	4,
	9,
	16
};

namespace Infinity
{
	unsigned int GetBytes(DataType type)
	{
		return BYTE_NUMS[(unsigned int)type];
	}

	unsigned int GetCount(DataType type)
	{
		return COUNTS[(unsigned int)type];
	}
}