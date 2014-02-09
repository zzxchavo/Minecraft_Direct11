
cbuffer  Matrixs : register(b1)
{
	matrix ProjectionMatrix;
	matrix ViewMatrix;
	matrix WorldMatrix;
};

cbuffer ObjectMatrix : register(b2)
{
	matrix Translation;
}
struct VSInputType
{
	 float4 position : POSITION;
	 float3 normal   : NORMAL;
	 float2 tex		 : TEXCOORD0;
};


struct PSInputType
{
	float4 position : SV_POSITION;
	float3 normal   : NORMAL;
    float2 tex		: TEXCOORD0;
};

PSInputType VSEntry(VSInputType input)
{
	PSInputType output;
	output.position = input.position;
	output.normal	= input.normal;
	output.tex		= input.tex;
	output.position = mul(output.position,Translation);
//    output.position = mul(output.position,WorldMatrix);
    output.position = mul(output.position,ViewMatrix);
    output.position = mul(output.position,ProjectionMatrix);
	return output;
}
