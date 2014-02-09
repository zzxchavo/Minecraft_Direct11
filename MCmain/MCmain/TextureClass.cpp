#include "TextureClass.h"


TextureClass::TextureClass(void)
{
	m_texture = NULL;
	m_SamplerState = NULL;
}


TextureClass::~TextureClass(void)
{
	if(m_texture)
	{
		m_texture ->Release();
		m_texture = NULL;
	}
	if(m_SamplerState)
	{
		m_SamplerState ->Release();
		m_SamplerState = NULL;
	}
}

bool TextureClass::Initialize(ID3D11Device * device,WCHAR *filename)
{
	HRESULT result;
	result = D3DX11CreateShaderResourceViewFromFile(device,filename,NULL,NULL,&m_texture,NULL);
	if(FAILED(result))
	{
		MessageBox(NULL,L"CreateShaderResource Failed!",L"Error - TextureClass",MB_OK);
		return false;
	}
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc,sizeof(D3D11_SAMPLER_DESC));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	result = device->CreateSamplerState(&sampDesc,&m_SamplerState);
	if(FAILED(result))
	{
		MessageBox(NULL,L"Create SamplerState Failed!",L"Error - TextureClass",MB_OK);
		return false;
	}
	return true;
}

void TextureClass::Shutdown()
{
	if(m_texture)
	{
		m_texture ->Release();
		m_texture = NULL;
	}
	if(m_SamplerState)
	{
		m_SamplerState ->Release();
		m_SamplerState = NULL;
	}
}

ID3D11ShaderResourceView *TextureClass::GetTexResource()
{
	return m_texture;
}

ID3D11SamplerState *TextureClass::GetTexSamplerState()
{
	return m_SamplerState;
}

void TextureClass::PSBindTexture(ID3D11Device * device,ID3D11DeviceContext * context)
{
	context->PSSetShaderResources(0,1,&m_texture);
	context->PSSetSamplers(0,1,&m_SamplerState);
}