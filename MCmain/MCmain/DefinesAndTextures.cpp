#include "DefinesAndTextures.h"


DefinesAndTextures::DefinesAndTextures(void)
{
	for(int i=0;i<MAX_TEX_NUM;i++)
		m_texs[i] = new TextureClass;
}


DefinesAndTextures::~DefinesAndTextures(void)
{
	
}

bool DefinesAndTextures::Initialize(ID3D11Device *device,ID3D11DeviceContext *context)
{
	HRESULT hr;
	hr = m_texs[COBBLE_STONE]->Initialize(device,L"data/default_cobble.png");if(FAILED(hr)) return false;
	hr = m_texs[SAND		]->Initialize(device,L"data/default_sand.png");if(FAILED(hr)) return false;
	hr = m_texs[GRASS_TOP	]->Initialize(device,L"data/default_grass.png");if(FAILED(hr)) return false;
	hr = m_texs[DIRT		]->Initialize(device,L"data/default_dirt.png");if(FAILED(hr)) return false;
	hr = m_texs[SKYBOX_DAY	]->Initialize(device,L"data/skybox2.png");if(FAILED(hr)) return false;
	hr = m_texs[SKYBOX_NIGHT]->Initialize(device,L"data/skybox2_night.png");if(FAILED(hr)) return false;
	hr = m_texs[TREE		]->Initialize(device,L"data/default_tree.png");if(FAILED(hr)) return false;
	hr = m_texs[STONE		]->Initialize(device,L"data/default_stone.png");if(FAILED(hr)) return false;
	hr = m_texs[GRAVEL		]->Initialize(device,L"data/default_gravel.png");if(FAILED(hr)) return false;
	hr = m_texs[CLAY		]->Initialize(device,L"data/default_clay.png");if(FAILED(hr)) return false;
	hr = m_texs[WATER		]->Initialize(device, L"data/default_water.png"); if (FAILED(hr)) return false;
	hr = m_texs[CLOUD		]->Initialize(device, L"data/default_cloud.png"); if (FAILED(hr)) return false;

	return true;
}	

TextureClass* DefinesAndTextures::operator [](const int index)const 
{
	return m_texs[index];
}

TextureClass* DefinesAndTextures::GetTextureClass(const int index)
{
	return m_texs[index];
}