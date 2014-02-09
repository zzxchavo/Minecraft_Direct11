#pragma once
#include<D3D11.h>
#include<D3DX11.h>

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();
	UINT GetX();
	UINT GetY();
	ID3D11DepthStencilView*& GetDepthStencilView();
	ID3D11RenderTargetView*& GetRenderTargetView();
	HRESULT Initialize(ID3D11Device* device,ID3D11DeviceContext* context,UINT width,UINT height,float near,float far);
	void ShutDown();

	void SetRenderTarget(ID3D11Device* device,ID3D11DeviceContext* context);
	void ClearRenderTarget(ID3D11Device* device, ID3D11DeviceContext* context,float rgba[]);
private:
	UINT _width, _height;
	ID3D11DepthStencilView* m_depth;
	ID3D11RenderTargetView* m_target;
	ID3D11Texture2D*		m_targetTex;
	ID3D11Texture2D*		m_depthTex; 
	ID3D11ShaderResourceView* resource;
	D3D11_VIEWPORT			m_viewport;
};

