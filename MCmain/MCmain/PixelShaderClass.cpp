#include "PixelShaderClass.h"


PixelShaderClass::PixelShaderClass(void)
{
	shader = NULL;
	pBlobOutput = NULL;
}


PixelShaderClass::~PixelShaderClass(void)
{
	if(shader == NULL)
	{
		shader->Release();
		shader = NULL;
	}
	if(pBlobOutput)
	{
		pBlobOutput->Release();
		pBlobOutput = NULL;
	}
}

HRESULT PixelShaderClass::Initialize
	(ID3D11Device * device,
	ID3D11DeviceContext * context,
	WCHAR * filename,
	char * Entry,
	char * shaderModel)
{
	HRESULT hr;
	hr = CompileShaderFromFile(filename,Entry,shaderModel,&pBlobOutput);
	if(FAILED(hr))
		return hr;
	hr = device->CreatePixelShader
		(pBlobOutput->GetBufferPointer(),
		pBlobOutput->GetBufferSize(),
		NULL,
		&shader);
	if(FAILED(hr))
		return hr;
	return S_OK;
}

HRESULT PixelShaderClass::CompileShaderFromFile
	(WCHAR* szFileName,
	LPCSTR szEntryPoint,
	LPCSTR szShaderModel,
	ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DX11CompileFromFile( szFileName, NULL, NULL, szEntryPoint, szShaderModel, 
		dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL );
	if( FAILED(hr) )
	{
		if( pErrorBlob != NULL )
			OutError(pErrorBlob);
		if( pErrorBlob ) pErrorBlob->Release();
		
		return hr;
	}
	if( pErrorBlob ) pErrorBlob->Release();
	return S_OK;
}

ID3D11PixelShader * PixelShaderClass::GetPixelShader()
{
	return shader;
}

void PixelShaderClass::ShutDown()
{
	if(shader)
	{
		shader->Release();
		shader = NULL;
	}
	if(pBlobOutput)
	{
		pBlobOutput->Release();
		pBlobOutput = NULL;
	}
}

void PixelShaderClass::OutError(ID3DBlob *error)
{
	char compileErrors[1000];
	memcpy(compileErrors,(char *) error->GetBufferPointer(),error->GetBufferSize());
	MessageBoxA(NULL,compileErrors,compileErrors,MB_OK);
}