#pragma once
#include"MapGenerateClass.h"
#include"Defines.h"
#include<D3DX10math.h>

#define BKDHASH(a,b) ((pmap)->BKDHash((a),(b)))

class OcclusionClass
{
public:
	OcclusionClass();
	~OcclusionClass();

	HRESULT Initialize(ID3D11Device*,
		ID3D11DeviceContext*,
		MapGenerateClass* );
	
	bool RenderFrame(float,float,float,int,int,int);

private:
	MapGenerateClass* pmap;
};
