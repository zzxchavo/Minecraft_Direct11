#pragma once
#include<D3D11.h>
#include<D3DX11.h>
#include"DefinesAndTextures.h"
class Render2TextureClass
{
public:
	Render2TextureClass();
	~Render2TextureClass();

	HRESULT Initialize(ID3D11Device* device,ID3D11DeviceContext* context);
	void Shutdown();

	void SetRenderTarget(ID3D11Device* device, ID3D11DeviceContext* context, ID3D11DepthStencilView* depth);
	void ClearRenderTarget(ID3D11Device* device,ID3D11DeviceContext* context,ID3D11DepthStencilView* depth);
	
	ID3D11ShaderResourceView*& GetShaderResourceView();
private:
	ID3D11Texture2D* m_renderTargetTexture;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11ShaderResourceView* m_shaderResourceView;
};

