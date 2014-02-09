#pragma once

#include <D3D11.h>
#include <D3DX11.h>
#include <d3dx11async.h>
#include <D3Dcompiler.h>

class VertexShaderClass
{
public:
	VertexShaderClass(void);
	~VertexShaderClass(void);

	HRESULT Initialize(ID3D11Device*,
		ID3D11DeviceContext*,
		WCHAR *,
		char *,
		char *,
		D3D11_INPUT_ELEMENT_DESC [],
		UINT);

	HRESULT CompileShaderFromFile( WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut );

	ID3D11VertexShader *GetVertexShader();

	void ShutDown();
	
	void OutError(ID3DBlob *);
private:
	ID3D11VertexShader * shader;
	ID3DBlob *pBlobOutput;
	ID3D11InputLayout*  g_pVertexLayout;
};

