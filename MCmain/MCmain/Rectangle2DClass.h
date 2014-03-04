#pragma once
#include<D3D11.h>
#include <memory.h>
#include <d3dx10math.h>
#include <stdio.h>
#include "VertexClass.h"
#include "OBBClass.h"
#include "DefinesAndTextures.h"

class Rectangle2DClass
{
public:
	Rectangle2DClass(void);
	~Rectangle2DClass(void);
	bool Initialize(ID3D11Device *, ID3D11DeviceContext *, int screenWidth, int screenHeight, int bitmapWidth, int bitmapHeight);
	HRESULT InitializeBuffers(ID3D11Device* device);
	HRESULT UpdateBuffers(ID3D11DeviceContext* context,int posX,int posY);
	HRESULT RenderBuffers(ID3D11DeviceContext* context);
	void Render(ID3D11Device *, ID3D11DeviceContext*,int,int);
	void SetPosition(float, float, float, ID3D11Device *, ID3D11DeviceContext*);
	void SetRotation(float, float, float, ID3D11Device*, ID3D11DeviceContext*);
	void SetScaling(float, float, float, ID3D11Device *, ID3D11DeviceContext *);

	void SetTransparency(ID3D11Device*, ID3D11DeviceContext*, float);
private:
	friend class SkyClass;
	friend class Rectangle2DClass;
	VertexClass pts[12];
	D3DXVECTOR3  pos, scaling, rotation;
	ID3D11Buffer* m_BlockBuffer;
	ID3D11Buffer* m_indexBuffer;
	struct Propertys
	{
		D3DXMATRIX	 TranslationMatrix;
		D3DXMATRIX   ScalingMatrix;
		D3DXMATRIX	 RotationMatrix;
	}propertys;
	ID3D11Buffer * m_BlockProperty;
	ID3D11Buffer * m_RenderFactors;
	struct Factors
	{
		float transparent;
		float rev1;
		float rev2;
		float rev3;
	}factors;
	int m_vertexCount, m_indexCount;
	int m_screenWidth, m_screenHeight;
	int m_bitmapWidth, m_bitmapHeight;
	int m_previousPosX, m_previousPosY;
};