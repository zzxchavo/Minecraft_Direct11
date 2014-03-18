#pragma once
#include"ArtEffect.h"
#include"Render2TextureClass.h"
#include"Rectangle2DClass.h"
#include"ShaderManager.h"

class DepthfieldEffect:public ArtEffect
{
public:
	DepthfieldEffect();
	~DepthfieldEffect();
	HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
	HRESULT UseEffect(ID3D11Device* device, ID3D11DeviceContext* context, ID3D11DepthStencilView* depthstencil, ID3D11ShaderResourceView* tSRV);
	HRESULT Draw(ID3D11Device* device, ID3D11DeviceContext* context);
	ID3D11ShaderResourceView*& GetShaderResourceView();
	HRESULT Shutdown();
private:
	Render2TextureClass* colorBuffer, *horizon, *vertical;
	Rectangle2DClass* m_screenRect;
};

