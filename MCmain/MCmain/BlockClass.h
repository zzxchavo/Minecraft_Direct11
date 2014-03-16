#pragma once
#include<D3D11.h>
#include <memory.h>
#include <d3dx10math.h>
#include <stdio.h>
#include "VertexClass.h"
#include "OBBClass.h"
#include "Defines.h"

class BlockClass
{
public:
	BlockClass(void);
	~BlockClass(void);
	HRESULT Initialize(ID3D11Device*,ID3D11DeviceContext*);
	void Render(ID3D11Device*,ID3D11DeviceContext*);
	void SetPosition(float,float,float,ID3D11Device*,ID3D11DeviceContext*);
	void SetRotation(float,float,float,ID3D11Device*,ID3D11DeviceContext*);
	void SetScaling(float,float,float,ID3D11Device*,ID3D11DeviceContext *);

	void SetTransparency(ID3D11Device*,ID3D11DeviceContext*,float);
private:
	friend class SkyClass;
	friend class Rectangle2DClass;
	VertexClass pts[36];
	D3DXVECTOR3  pos,scaling,rotation;
	struct Propertys
	{
		D3DXMATRIX	 TranslationMatrix;
		D3DXMATRIX   ScalingMatrix;
		D3DXMATRIX	 RotationMatrix;
	}propertys;
	struct Factors
	{
		float transparent;
		float rev1;
		float rev2;
		float rev3;
	}factors;
	ID3D11Buffer* m_BlockBuffer;
	ID3D11Buffer* m_BlockProperty;
	ID3D11Buffer* m_RenderFactors;
};