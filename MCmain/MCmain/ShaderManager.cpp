#include "ShaderManager.h"

ShaderManager* ShaderManager::instance = nullptr;

ShaderManager& ShaderManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new ShaderManager;
		return *instance;
	}
	return *instance;
}

ShaderManager::ShaderManager()
{
}


ShaderManager::~ShaderManager()
{
}


HRESULT ShaderManager::Initialize(ID3D11Device*& device, ID3D11DeviceContext*& context)
{
	HRESULT hr = S_OK;

	return hr;
}

HRESULT ShaderManager::ShutDown()
{
	HRESULT hr = S_OK;
	for (auto& _param : vec)
	{
		DELETESHADER(_param.vs);
		DELETESHADER(_param.ps);
	}
	vec.clear();
	return hr;
}

HRESULT ShaderManager::AddShader(ID3D11Device* device,ID3D11DeviceContext* context,std::string ShaderName, WCHAR* vsPos, WCHAR* psPos, D3D11_INPUT_ELEMENT_DESC* layout,UINT num)
{
	HRESULT hr;
	GroupShader imagin;
	vec.push_back(imagin);
	mp[ShaderName] = (int)vec.size() - 1;
	vec.back().ShaderName = ShaderName;
	CREATE_VERTEX_SHADER(vec.back().vs);
	CREATE_PIXEL_SHADER(vec.back().ps);
	hr = vec.back().vs->Initialize(device, context,vsPos,"VSEntry","vs_5_0",layout,num);
	if (FAILED(hr)) return hr;
	hr = vec.back().ps->Initialize(device, context, psPos, "PSEntry", "ps_5_0");
	if (FAILED(hr)) return hr;
	return S_OK;
}

HRESULT ShaderManager::UseShader(ID3D11Device* device, ID3D11DeviceContext* context, std::string ShaderName)
{
	context->VSSetShader(vec[mp[ShaderName]].vs->GetVertexShader(), NULL, 0);
	context->PSSetShader(vec[mp[ShaderName]].ps->GetPixelShader(), NULL, 0);
	return S_OK;
}

unsigned int ShaderManager::GetSize()
{
	return vec.size();
}