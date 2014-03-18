#include "BlurEffect.h"

BlurEffect::BlurEffect()
{
	SAFE_CREATE(colorBuffer, Render2TextureClass);
	SAFE_CREATE(horizon, Render2TextureClass);
	SAFE_CREATE(vertical, Render2TextureClass);
	SAFE_CREATE(m_screenRect, Rectangle2DClass);
}

BlurEffect::~BlurEffect()
{
	SAFE_DELETE(colorBuffer);
	SAFE_DELETE(horizon);
	SAFE_DELETE(vertical);
	SAFE_DELETE(m_screenRect);
}

HRESULT BlurEffect::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
	HRESULT hr = S_OK;
	colorBuffer->Initialize(device,context);
	horizon->Initialize(device,context);
	vertical->Initialize(device, context);
	m_screenRect->Initialize(device, context, screenWidth, screenHeight, 800, 600);
	return hr;
}

HRESULT BlurEffect::UseEffect(ID3D11Device* device, 
	ID3D11DeviceContext* context, 
	ID3D11DepthStencilView* depthstencil,
	ID3D11ShaderResourceView* tSRV)
{
	HRESULT hr = S_OK;
	colorBuffer->SetRenderTarget(device, context, depthstencil);
	colorBuffer->ClearRenderTarget(device, context, depthstencil);
	//Render The Scene
	context->PSSetShaderResources(0, 1, &tSRV);
	SHADERMANAGER.UseShader(device, context, "tex2d");
	m_screenRect->Render(device, context, 0, 0);

	horizon->SetRenderTarget(device, context, depthstencil);
	horizon->ClearRenderTarget(device, context, depthstencil);

	SHADERMANAGER.UseShader(device, context, "horizontal");
	context->PSSetShaderResources(0, 1, &(colorBuffer->GetShaderResourceView()));
	m_screenRect->Render(device, context, 0, 0);
	
	vertical->SetRenderTarget(device,context,depthstencil);
	vertical->ClearRenderTarget(device, context, depthstencil);
	
	colorBuffer->SetRenderTarget(device, context, depthstencil);
	colorBuffer->ClearRenderTarget(device, context, depthstencil);
	SHADERMANAGER.UseShader(device, context, "vertical");
	context->PSSetShaderResources(0, 1, &(horizon->GetShaderResourceView()));
	m_screenRect->Render(device,context,0,0);
	return hr;
}

HRESULT BlurEffect::Draw(ID3D11Device* device, ID3D11DeviceContext* context)
{
	HRESULT hr = S_OK;
	;
	return hr;
}
ID3D11ShaderResourceView*& BlurEffect::GetShaderResourceView()
{
	return colorBuffer->GetShaderResourceView();
}
HRESULT BlurEffect::Shutdown()
{
	HRESULT hr = S_OK;
	colorBuffer->Shutdown();
	horizon->Shutdown();
	vertical->Shutdown();
	return hr;
}