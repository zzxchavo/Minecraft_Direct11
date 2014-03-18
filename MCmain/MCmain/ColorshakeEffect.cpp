#include "ColorshakeEffect.h"


ColorshakeEffect::ColorshakeEffect()
{
	SAFE_CREATE(colorBuffer, Render2TextureClass);
	SAFE_CREATE(m_screenRect, Rectangle2DClass);
}

ColorshakeEffect::~ColorshakeEffect()
{
	SAFE_DELETE(colorBuffer);
	SAFE_DELETE(m_screenRect);
}


HRESULT ColorshakeEffect::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
	HRESULT hr = S_OK;
	colorBuffer->Initialize(device, context);
	m_screenRect->Initialize(device, context, screenWidth, screenHeight, 800, 600);
	return hr;
}

HRESULT ColorshakeEffect::UseEffect(ID3D11Device* device,
	ID3D11DeviceContext* context,
	ID3D11DepthStencilView* depthstencil,
	ID3D11ShaderResourceView* tSRV)
{
	HRESULT hr = S_OK;
	colorBuffer->SetRenderTarget(device, context, depthstencil);
	colorBuffer->ClearRenderTarget(device, context, depthstencil);
	//Render The Scene
	context->PSSetShaderResources(0, 1, &tSRV);
	SHADERMANAGER.UseShader(device, context, "colorshake");
	m_screenRect->Render(device, context, 0, 0);
	return hr;
}

HRESULT ColorshakeEffect::Draw(ID3D11Device* device, ID3D11DeviceContext* context)
{
	HRESULT hr = S_OK;
	;
	return hr;
}
ID3D11ShaderResourceView*& ColorshakeEffect::GetShaderResourceView()
{
	return colorBuffer->GetShaderResourceView();
}
HRESULT ColorshakeEffect::Shutdown()
{
	HRESULT hr = S_OK;
	colorBuffer->Shutdown();
	return hr;
}