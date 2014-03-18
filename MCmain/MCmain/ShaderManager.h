#pragma once

#include<D3DX11.h>
#include<map>
#include<vector>
#include<memory>
#include<string>
#include"Defines.h"
#include"VertexShaderClass.h"
#include"PixelShaderClass.h"

class ShaderManager
{
public:
	static ShaderManager& GetInstance();
	HRESULT Initialize(ID3D11Device*& device,ID3D11DeviceContext*& context);
	HRESULT UseShader(ID3D11Device* device, ID3D11DeviceContext* context, std::string ShaderName);
	HRESULT AddShader(ID3D11Device* device, ID3D11DeviceContext* context, std::string ShaderName, WCHAR* vsPos, WCHAR* psPos, D3D11_INPUT_ELEMENT_DESC* layout, UINT num);
	HRESULT ShutDown();

	unsigned int GetSize();
private:
	struct GroupShader
	{
		VertexShaderClass* vs;
		PixelShaderClass* ps;
		std::string ShaderName;	
	};

	ShaderManager();
	~ShaderManager();
	static ShaderManager* instance;
	ID3D11Device* pdevice;
	ID3D11DeviceContext* pcontext;
	std::vector<GroupShader> vec;
	std::map<std::string,unsigned int> mp;
};
#define SHADERMANAGER (ShaderManager::GetInstance())