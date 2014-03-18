#include "DepthfieldEffect.h"


DepthfieldEffect::DepthfieldEffect()
{
	SAFE_CREATE(colorBuffer, Render2TextureClass);
	SAFE_CREATE(horizon, Render2TextureClass);
	SAFE_CREATE(vertical, Render2TextureClass);
	SAFE_CREATE(m_screenRect, Rectangle2DClass);
}


DepthfieldEffect::~DepthfieldEffect()
{
	SAFE_DELETE(colorBuffer);
	SAFE_DELETE(horizon);
	SAFE_DELETE(vertical);
	SAFE_DELETE(m_screenRect);
}


HRESULT DepthfieldEffect::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
	HRESULT hr = S_OK;
	m_screenRect->Initialize(device, context, screenWidth, screenHeight, 800, 600);
	vertical->Initialize(device, context);
	horizon->Initialize(device, context);
	colorBuffer->Initialize(device, context);
	return hr;
}

HRESULT DepthfieldEffect::Shutdown()
{
	colorBuffer->Shutdown();
	vertical->Shutdown();
	horizon->Shutdown();
	return S_OK;
}

HRESULT DepthfieldEffect::UseEffect(ID3D11Device* device, ID3D11DeviceContext* context,
	ID3D11DepthStencilView* depthstencil, ID3D11ShaderResourceView* tSRV)
{
	HRESULT hr = S_OK;

	colorBuffer->SetRenderTarget(device, context, depthstencil);
	colorBuffer->ClearRenderTarget(device, context, depthstencil);
	context->PSSetShaderResources(0, 1, &tSRV);
	SHADERMANAGER.UseShader(device, context, "get_depth");
	m_screenRect->Render(device, context, 0, 0);
	
	horizon->SetRenderTarget(device, context, depthstencil);
	horizon->ClearRenderTarget(device, context, depthstencil);

	SHADERMANAGER.UseShader(device, context, "horizontal");
	context->PSSetShaderResources(0, 1, &(colorBuffer->GetShaderResourceView()));
	m_screenRect->Render(device, context, 0, 0);

	vertical->SetRenderTarget(device, context, depthstencil);
	vertical->ClearRenderTarget(device, context, depthstencil);
	SHADERMANAGER.UseShader(device, context, "vertical");
	context->PSSetShaderResources(0, 1, &(horizon->GetShaderResourceView()));
	m_screenRect->Render(device, context, 0, 0);

	colorBuffer->SetRenderTarget(device, context, depthstencil);
	colorBuffer->ClearRenderTarget(device, context, depthstencil);
	SHADERMANAGER.UseShader(device, context, "depth");
	context->PSSetShaderResources(0, 1, &tSRV);
	context->PSSetShaderResources(1, 1, &(vertical->GetShaderResourceView()));
	m_screenRect->Render(device, context, 0, 0);
	return hr;
}

HRESULT DepthfieldEffect::Draw(ID3D11Device* device, ID3D11DeviceContext* context)
{
	return S_OK;
}

ID3D11ShaderResourceView*& DepthfieldEffect::GetShaderResourceView()
{
	return colorBuffer->GetShaderResourceView();
}
