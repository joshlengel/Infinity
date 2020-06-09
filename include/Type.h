#pragma once

namespace Infinity
{
	enum class DataType : unsigned char
	{
		INT,
		INT2,
		INT3,
		INT4,

		FLOAT,
		FLOAT2,
		FLOAT3,
		FLOAT4,

		DOUBLE,
		DOUBLE2,
		DOUBLE3,
		DOUBLE4,

		MAT3,
		MAT4
	};

	unsigned int GetBytes(DataType type);
	unsigned int GetCount(DataType type);
}