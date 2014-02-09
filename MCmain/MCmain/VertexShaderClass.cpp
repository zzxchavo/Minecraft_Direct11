#include "VertexShaderClass.h"


VertexShaderClass::VertexShaderClass(void)
{
	shader = NULL;
	pBlobOutput = NULL;
	g_pVertexLayout = NULL;
}


VertexShaderClass::~VertexShaderClass(void)
{
	if(shader)
	{
		shader->Release();
		shader = NULL;
	}
	if(pBlobOutput)
	{
		pBlobOutput -> Release();
		pBlobOutput = NULL;
	}
	if(g_pVertexLayout)
	{
		g_pVertexLayout->Release();
		g_pVertexLayout = NULL;
	}
}


HRESULT VertexShaderClass::Initialize
	(ID3D11Device * device,
	ID3D11DeviceContext * context,
	WCHAR *filename,
	char *Entry,
	char *shaderModel,
	D3D11_INPUT_ELEMENT_DESC layout[] ,
	UINT numElements)
{
	HRESULT hr = S_OK;
	hr = CompileShaderFromFile(
		filename,
		Entry,
		shaderModel,
		&pBlobOutput);
	if(FAILED(hr))
		return hr;
	hr = device->CreateVertexShader(pBlobOutput->GetBufferPointer(),
		pBlobOutput->GetBufferSize(),
		NULL,
		&shader);
	if(FAILED(hr))
		return hr;
	hr = device->CreateInputLayout(layout,numElements,
		pBlobOutput->GetBufferPointer(),
		pBlobOutput->GetBufferSize(),
		&g_pVertexLayout);
	pBlobOutput->Release();
	pBlobOutput = NULL;
	if(FAILED(hr))
		return hr;
	context->IASetInputLayout(g_pVertexLayout);
	return S_OK;
}


HRESULT VertexShaderClass::CompileShaderFromFile
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
		{
			OutError(pErrorBlob);
			pErrorBlob->Release();
		}
		return hr;
	}
	if( pErrorBlob ) 
		pErrorBlob->Release();
	return S_OK;
}


ID3D11VertexShader * VertexShaderClass::GetVertexShader()
{
	return shader;
}


void VertexShaderClass::ShutDown()
{
	if(shader)
	{
		shader->Release();
		shader = NULL;
	}
	if(pBlobOutput)
	{
		pBlobOutput -> Release();
		pBlobOutput = NULL;
	}
}

void VertexShaderClass::OutError(ID3DBlob *error)
{
	char compileErrors[1000];
	memcpy(compileErrors,(char *) error->GetBufferPointer(),error->GetBufferSize());
	MessageBoxA(NULL,compileErrors,compileErrors,MB_OK);
}