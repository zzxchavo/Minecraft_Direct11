#pragma once

#include <D3D11.h>
#include <D3DX11.h>
#include <d3dx11async.h>
#include <D3Dcompiler.h>
#include <memory.h>


class PixelShaderClass
{
public:
	PixelShaderClass(void);
	~PixelShaderClass(void);

	HRESULT Initialize(ID3D11Device*,ID3D11DeviceContext*,WCHAR *,char *,char *);

	HRESULT CompileShaderFromFile( WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut );

	ID3D11PixelShader *GetPixelShader();

	void ShutDown();

	void OutError(ID3DBlob *);
	

private:
	ID3D11PixelShader * shader;
	ID3DBlob * pBlobOutput;
};

