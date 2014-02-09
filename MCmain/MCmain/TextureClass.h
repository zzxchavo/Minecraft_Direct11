#pragma once
#include <D3D11.h>
#include <d3dx11tex.h>

class TextureClass
{
public:
	TextureClass(void);
	~TextureClass(void);
	bool Initialize(ID3D11Device *,WCHAR *);
	void Shutdown();
	void PSBindTexture(ID3D11Device *,ID3D11DeviceContext *);
	ID3D11ShaderResourceView * GetTexResource();
	ID3D11SamplerState		 * GetTexSamplerState();
private:
	ID3D11ShaderResourceView *m_texture;
	ID3D11SamplerState		 *m_SamplerState;
};
