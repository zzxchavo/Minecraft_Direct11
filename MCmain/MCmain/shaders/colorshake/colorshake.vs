
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
	float2 texr:TEXCOORD1;
	float2 texg:TEXCOORD2;
	float2 texb:TEXCOORD3;
};
PSInputType VSEntry(VSInputType input)
{
	PSInputType output;
	input.position.w = 1.0;
	float ScreenWidth = 800.0f,ScreenHeight = 600.0f;
	output.position.x = input.position.x;
	output.position.y = input.position.y;
	output.position.z = 0.0;
	output.position.w = 1.0;
	output.tex = input.tex;
	float texelSize = 1.0f / ScreenHeight;
	output.texr = input.tex + float2(-6.0f * texelSize,texelSize * -7.0f);
	output.texg = input.tex + float2(6.0f * texelSize,texelSize * -3.0f);
	output.texb = input.tex + float2(-6.0f * texelSize,texelSize * 4.0f);
	return output;
}