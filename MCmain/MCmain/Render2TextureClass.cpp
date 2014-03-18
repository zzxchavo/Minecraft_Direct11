#include "Render2TextureClass.h"


Render2TextureClass::Render2TextureClass()
{
	m_renderTargetTexture = nullptr;
	m_shaderResourceView = nullptr;
	m_renderTargetView = nullptr;
}


Render2TextureClass::~Render2TextureClass()
{
}


HRESULT Render2TextureClass::Initialize(ID3D11Device* device,ID3D11DeviceContext* context)
{
	HRESULT hr;
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory(&textureDesc,sizeof(textureDesc));
	ZeroMemory(&renderTargetViewDesc,sizeof(renderTargetViewDesc));
	ZeroMemory(&shaderResourceViewDesc,sizeof(shaderResourceViewDesc));
	textureDesc.Width = 800;
	textureDesc.Height = 600;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET|D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	hr = device->CreateTexture2D(&textureDesc,NULL,&m_renderTargetTexture);
	if (FAILED(hr))
		return hr;
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;
	hr = device->CreateRenderTargetView(m_renderTargetTexture, &renderTargetViewDesc, &m_renderTargetView);
	if (FAILED(hr))
		return hr;
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	hr = device->CreateShaderResourceView(m_renderTargetTexture, &shaderResourceViewDesc, &m_shaderResourceView);
	if (FAILED(hr))
		return hr;
	return S_OK;
}

void Render2TextureClass::Shutdown()
{
	SAFE_RELEASE(m_shaderResourceView);
	SAFE_RELEASE(m_renderTargetTexture);
	SAFE_RELEASE(m_renderTargetView);
}

void Render2TextureClass::SetRenderTarget(
	ID3D11Device* device,
	ID3D11DeviceContext* context, 
	ID3D11DepthStencilView* depth)
{
	context->OMSetRenderTargets(1,&m_renderTargetView,depth);
	return;
}

void Render2TextureClass::ClearRenderTarget(
	ID3D11Device* device, 
	ID3D11DeviceContext* context, 
	ID3D11DepthStencilView* depth)
{
	float clearColor[] = {0.0f,0.0f,0.0f,1.0f};
	context->ClearRenderTargetView(m_renderTargetView,clearColor);
	context->ClearDepthStencilView(depth,D3D11_CLEAR_DEPTH , 1.0f , NULL);
	return ;
}

ID3D11ShaderResourceView*& Render2TextureClass::GetShaderResourceView()
{
	return m_shaderResourceView;
}