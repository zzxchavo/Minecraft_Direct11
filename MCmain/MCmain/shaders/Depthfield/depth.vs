
cbuffer  Matrixs : register(b1)
{
	matrix ProjectionMatrix;
	matrix ViewMatrix;
	matrix WorldMatrix;
};

cbuffer ObjectMatrix : register(b2)
{
	matrix Translation;
	matrix Scaling;
	matrix Rotation;
};

struct VSInputType
{
	float4 position:POSITION;
	float3 normal:NORMAL;
	float2 tex:TEXCOORD0;
};
struct PSInputType
{
	float4 position:SV_POSITION;
	float2 tex:TEXCOORD0;
};
PSInputType VSEntry(VSInputType input)
{
	PSInputType output;
	input.position.w = 1.0;
	output.position.x = input.position.x;
	output.position.y = input.position.y;
	output.position.z = 0.0;
	output.position.w = 1.0;
	output.tex = input.tex;
	return output;
}