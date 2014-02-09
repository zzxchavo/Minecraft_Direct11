
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
	float4 depthPosition : TEXCOORD1;
	float DisappearFactor : CLOUD;
};

PSInputType VSEntry(VSInputType input)
{
	float4 cameraPosition;
	input.position.w = 1.0f;
	PSInputType output;
	output.position = input.position;
	output.normal	= input.normal;
	output.tex		= input.tex;
	output.position = mul(output.position,Scaling);
	output.position = mul(output.position,Translation);
//  output.position = mul(output.position,WorldMatrix);
    output.position = mul(output.position,ViewMatrix);
	cameraPosition  = output.position;
    output.position = mul(output.position,ProjectionMatrix);
	output.normal	= mul(output.normal,WorldMatrix);
	output.normal	= normalize(output.normal);
	output.depthPosition = output.position;
	float FogEnd = 50.0f;
	float FogStart = 25.0f;
	output.DisappearFactor = 1.0f + saturate((FogEnd - cameraPosition.z)/(FogEnd - FogStart));
	return output;
}
