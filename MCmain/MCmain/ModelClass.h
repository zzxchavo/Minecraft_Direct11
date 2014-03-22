#pragma once
#include<stdio.h>
#include<D3D11.h>
#include<D3DX11.h>
#include<D3DX10math.h>
#include<string>
#include<vector>
#include<map>
#include<string.h>
#include<regex>
#include<iostream>
#include"Defines.h"
#include"VertexClass.h"
using namespace std;

class ModelClass
{
public:
	ModelClass();
	~ModelClass();
	HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context, const char* filename);
	HRESULT Render(ID3D11Device* device, ID3D11DeviceContext* context);
	HRESULT Shutdown();

	void SetPosition(float, float, float, ID3D11Device*, ID3D11DeviceContext*);
	void SetRotation(float, float, float, ID3D11Device*, ID3D11DeviceContext*);
	void SetScaling(float, float, float, ID3D11Device*, ID3D11DeviceContext *);
	void SetTransparency(ID3D11Device *device, ID3D11DeviceContext * context, float trans);
private:
	HRESULT InitializeDatas(const char* filename);

	FILE *fp;
	struct FLOAT3{ float var[3]; };
	std::string* filetext;
	std::string* filetextcpy;
	vector<FLOAT3	>pos;
	vector<FLOAT3	>nor;
	vector<FLOAT3	>tex;
	vector<vector<VertexClass>* >vec;
	vector<std::string> name;
	map<std::string, size_t> mp;
	D3DXVECTOR3  modelpos, scaling, rotation;
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
	ULONG* indices;
	ID3D11Buffer** m_indexBuffer;
	ID3D11Buffer** m_ModelBuffer;
	ID3D11Buffer* m_ModelProperty;
	ID3D11Buffer* m_RenderFactors;
};

