#pragma once
#include"ArtEffect.h"
#include"BlurEffect.h"
#include"ColorshakeEffect.h"
#include"GlowEffect.h"
#include"DepthfieldEffect.h"
#include<D3D11.h>
#include<D3DX11.h>
#include<vector>
#include<map>
#include<memory>

class EffectManager
{
public:
	static EffectManager& GetInstance();
	HRESULT AddEffect(ID3D11Device* device, ID3D11DeviceContext* context, std::string EffectName);
	HRESULT Render(ID3D11Device* device, ID3D11DeviceContext* context,ID3D11DepthStencilView* depthstencil,ID3D11ShaderResourceView* tSRV);
	HRESULT ClearEffect();
	HRESULT Shutdown();
	ID3D11ShaderResourceView*& GetShaderResourceView();
private:
	std::vector<ArtEffect*> vec;
	std::map<std::string, size_t> mp;
	EffectManager();
	~EffectManager();
	static EffectManager* instance;
};

#define EFFECTMANAGER (EffectManager::GetInstance())
