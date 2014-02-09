#include "FrameBuffer.h"


FrameBuffer::FrameBuffer()
{
}


FrameBuffer::~FrameBuffer()
{
}

UINT FrameBuffer::GetX()
{
	return _width;
}

UINT FrameBuffer::GetY()
{
	return _height;
}

ID3D11DepthStencilView*& FrameBuffer::GetDepthStencilView()
{
	return m_depth;
}

ID3D11RenderTargetView*& FrameBuffer::GetRenderTargetView()
{
	return m_target;
}

void FrameBuffer::ShutDown()
{
	if (m_depth)
	{
		m_depth->Release();
		m_depth = nullptr;
	}
	if (m_target)
	{
		m_target->Release();
		m_target = nullptr;
	}
	if (m_targetTex)
	{
		m_targetTex->Release();
		m_targetTex = nullptr;
	}
	if (m_depthTex)
	{
		m_depthTex->Release();
		m_depthTex = nullptr;
	}
	if (resource)
	{
		resource->Release();
		resource = nullptr;
	}
}

HRESULT FrameBuffer::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, UINT width, UINT height, float near, float far)
{
	HRESULT hr = S_OK;
	_width = width;
	_height = height;
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	hr = device->CreateTexture2D(&textureDesc, NULL, &m_targetTex);
	if (FAILED(hr))
		return hr;
	D3D11_RENDER_TARGET_VIEW_DESC rendertargetdesc;
	ZeroMemory(&rendertargetdesc, sizeof(rendertargetdesc));
	rendertargetdesc.Format = textureDesc.Format;
	rendertargetdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rendertargetdesc.Texture2D.MipSlice = 0;

	hr = device->CreateRenderTargetView(m_targetTex, &rendertargetdesc, &m_target);
	if (FAILED(hr))
	{
		return hr;
	}

	D3D11_TEXTURE2D_DESC desc2D;
	ZeroMemory(&desc2D, sizeof(desc2D));
	desc2D.Width = width;
	desc2D.Height = height;
	desc2D.MipLevels = 1;
	desc2D.ArraySize = 1;
	desc2D.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc2D.SampleDesc.Count = 1;
	desc2D.SampleDesc.Quality = 0;
	desc2D.Usage = D3D11_USAGE_DEFAULT;
	desc2D.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc2D.CPUAccessFlags = 0;
	desc2D.MiscFlags = 0;
	hr = device->CreateTexture2D(&desc2D, NULL, &m_depthTex);
	if (FAILED(hr))
	{
		return hr;
	}

	m_viewport.Width = width;
	m_viewport.Height = height;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;
	
	
	D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc;
	ZeroMemory(&depthDesc,sizeof(depthDesc));
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthDesc.Texture2D.MipSlice = 0;
	hr = device->CreateDepthStencilView(m_depthTex,&depthDesc,&m_depth);
	if (FAILED(hr))
	{
		return hr;
	}
	D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc;
	ZeroMemory(&resourceDesc, sizeof(resourceDesc));
	//DXGI_FORMAT_R8G8B8A8_UNORM_SRGB
	resourceDesc.Format = textureDesc.Format;
	resourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resourceDesc.Texture2D.MostDetailedMip = 0;
	resourceDesc.Texture2D.MipLevels = 1;

	hr = device->CreateShaderResourceView(m_targetTex, &resourceDesc, &resource);
	if (FAILED(hr))
	{
		return hr;
	}
	return hr;
}


void FrameBuffer::SetRenderTarget(ID3D11Device* device, ID3D11DeviceContext* context)
{
	context->OMSetRenderTargets(1,&m_target,m_depth);
	context->RSSetViewports(1, &m_viewport);
}

void FrameBuffer::ClearRenderTarget(ID3D11Device* device,ID3D11DeviceContext* context,float rgba[])
{
	context->ClearRenderTargetView(m_target, rgba);
	context->ClearDepthStencilView(m_depth, D3D11_CLEAR_DEPTH, 1.0f, 0);
}