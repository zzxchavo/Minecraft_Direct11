#pragma once
#include<D3D11.h>
#include<D3DX11.h>

class ArtEffect
{
public:
	ArtEffect();
	~ArtEffect();
	virtual HRESULT Initialize(ID3D11Device* device,ID3D11DeviceContext* context) = 0;
	virtual HRESULT UseEffect(ID3D11Device* device, ID3D11DeviceContext* context, ID3D11DepthStencilView* depthstencil, ID3D11ShaderResourceView* tSRV) = 0;
	virtual HRESULT Shutdown() = 0;
	virtual ID3D11ShaderResourceView*& GetShaderResourceView() = 0;
};

