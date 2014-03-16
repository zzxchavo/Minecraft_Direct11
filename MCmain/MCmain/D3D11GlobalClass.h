#pragma once
#include<D3D11.h>
#include<D3DX11.h>

class D3D11GlobalClass
{
public:
	D3D11GlobalClass();
	~D3D11GlobalClass();
	HRESULT Initialize(HWND hwnd);
	HRESULT Shutdown();
	
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetContext();

private:
	D3D_DRIVER_TYPE         g_driverType;
	D3D_FEATURE_LEVEL       g_featureLevel;
	ID3D11Device*           g_pd3dDevice;
	ID3D11DeviceContext*    g_pImmediateContext;
	IDXGISwapChain*         g_pSwapChain;
	ID3D11Texture2D*        g_pDepthStencil;
	ID3D11DepthStencilView* g_pDepthStencilView;
	ID3D11RenderTargetView* g_pRenderTargetView;
	ID3D11RasterizerState*  raState;
	D3D11_VIEWPORT vp;
};

