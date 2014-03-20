#include "EffectManager.h"
EffectManager* EffectManager::instance = nullptr;

EffectManager::EffectManager()
{
}


EffectManager::~EffectManager()
{
}

EffectManager& EffectManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new EffectManager;
	}
	return *instance;
}

HRESULT EffectManager::AddEffect(ID3D11Device* device, ID3D11DeviceContext* context,std::string EffectName)
{
	HRESULT hr = S_OK;
	ArtEffect* imag;
	vec.push_back(imag);
	mp[EffectName] =(int)vec.size()-1;
	if (EffectName == "colorshake")
	{
		SAFE_CREATE(vec.back(),ColorshakeEffect);
	}
	else if (EffectName == "blur")
	{
		SAFE_CREATE(vec.back(),BlurEffect);
	}
	else if (EffectName == "glow")
	{
		SAFE_CREATE(vec.back(),GlowEffect);
	}
	else if (EffectName == "depthfield")
	{
		SAFE_CREATE(vec.back(),DepthfieldEffect);
	}
	if (FAILED(vec.back()->Initialize(device, context)))
	{
		return E_FAIL;
	}
	return hr;
}

HRESULT EffectManager::Render(ID3D11Device* device, ID3D11DeviceContext* context, ID3D11DepthStencilView* depthstencil, ID3D11ShaderResourceView* tSRV)
{
	HRESULT hr = S_OK;
	if (vec.size() == 0) return S_OK;
	vec[0]->UseEffect(device,context,depthstencil,tSRV);
	for (int i = 1;i < vec.size(); i++)
	{
		vec[i]->UseEffect(device,context,depthstencil,vec[i-1]->GetShaderResourceView());
	}
	return hr;
}

ID3D11ShaderResourceView*& EffectManager::GetShaderResourceView()
{
	return vec.back()->GetShaderResourceView();
}

HRESULT EffectManager::Shutdown()
{
	for (auto& _param : vec)
	{
		_param->Shutdown();
	}
	vec.clear();
	return S_OK;
}