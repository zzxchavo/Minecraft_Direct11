
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
	float2 tex1:TEXCOORD1;
	float2 tex2:TEXCOORD2;
	float2 tex3:TEXCOORD3;
	float2 tex4:TEXCOORD4;
	float2 tex5:TEXCOORD5;
	float2 tex6:TEXCOORD6;
	float2 tex7:TEXCOORD7;
	float2 tex8:TEXCOORD8;
	float2 tex9:TEXCOORD9;
	float2 tex10:TEXCOORD10;
	float2 tex11:TEXCOORD11;
	float2 tex12:TEXCOORD12;
	float2 tex13:TEXCOORD13;
	float2 tex14:TEXCOORD14;	
	float2 tex15:TEXCOORD15;
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
	float texelSize = 1.0f / ScreenWidth;
	output.tex1 = input.tex + float2(texelSize * -7.0f,0.0f);
	output.tex2 = input.tex + float2(texelSize * -6.0f,0.0f);
	output.tex3 = input.tex + float2(texelSize * -5.0f,0.0f);
	output.tex4 = input.tex + float2(texelSize * -4.0f,0.0f);
	output.tex5 = input.tex + float2(texelSize * -3.0f,0.0f);
	output.tex6 = input.tex + float2(texelSize * -2.0f,0.0f);
	output.tex7 = input.tex + float2(texelSize * -1.0f,0.0f);
	output.tex8 = input.tex + float2(texelSize *  0.0f,0.0f);
	output.tex9 = input.tex + float2(texelSize *  1.0f,0.0f);
	output.tex10 = input.tex + float2(texelSize *  2.0f,0.0f);
	output.tex11 = input.tex + float2(texelSize *  3.0f,0.0f);
	output.tex12 = input.tex + float2(texelSize *  4.0f,0.0f);
	output.tex13 = input.tex + float2(texelSize *  5.0f,0.0f);
	output.tex14 = input.tex + float2(texelSize *  6.0f,0.0f);
	output.tex15 = input.tex + float2(texelSize *  7.0f,0.0f);
	return output;
}