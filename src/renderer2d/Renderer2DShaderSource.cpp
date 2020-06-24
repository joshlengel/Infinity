#include"InfinityPCH.h"

constexpr static const char renderer2D_batched_v_source[] = R"(
cbuffer Constants
{
	matrix projection_view;
};

struct VertexIn
{
	float2 position : ATTRIBUTE0;
	float4 color : ATTRIBUTE1;
	float2 tex_coords : ATTRIBUTE2;
};

struct VertexOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR0;
	float2 tex_coords : TEXCOORD;
};

VertexOut main(VertexIn input)
{
	VertexOut output;
	
	output.position = mul(float4(input.position, 0.0, 1.0), projection_view);
	output.color = input.color;
	output.tex_coords = input.tex_coords;

	return output;
}
)";

constexpr static const char renderer2D_unbatched_v_source[] = R"(
cbuffer Constants
{
	matrix model;
	matrix projection_view;

	float4 color;
};

struct VertexIn
{
	float2 vertex : ATTRIBUTE0;
	float2 tex_coords : ATTRIBUTE1;
};

struct VertexOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR0;
	float2 tex_coords : TEXCOORD;
};

VertexOut main(VertexIn input)
{
	VertexOut output;
	
	output.position = mul(float4(input.vertex, 0.0, 1.0), model);
	output.position = mul(output.position, projection_view);
	output.color = color;
	output.tex_coords = input.tex_coords;

	return output;
}
)";

constexpr static const char renderer2D_p_source[] = R"(
Texture2D tex;
SamplerState texSampler;

struct PixelIn
{
	float4 position : SV_POSITION;
	float4 color : COLOR0;
	float2 tex_coords : TEXCOORD;
};

float4 main(PixelIn input) : SV_TARGET
{
	return tex.Sample(texSampler, input.tex_coords) * input.color;
}
)";