#pragma once

#include<stdio.h>
#include"TextureClass.h"

#define MAX_TEX_NUM   15

#define TEX_EMPTY     0
#define STONE         1
#define COBBLE_STONE  2
#define GRASS_TOP     3
#define DIRT	      4
#define GRAVEL		  5
#define SAND          6
#define CLAY		  7
#define SKYBOX_DAY    8
#define SKYBOX_NIGHT  9
#define SKYBOX_DAWN   10
#define WATER		  11
#define CLOUD		  12
#define TREE          13


class DefinesAndTextures
{
public:
	DefinesAndTextures();
	~DefinesAndTextures();
	bool Initialize(ID3D11Device *,ID3D11DeviceContext *);
	TextureClass* GetTextureClass(const int);
	TextureClass* operator [](const int)const ;
private:
	TextureClass * m_texs[MAX_TEX_NUM];
};



#define CREATE_VERTEX_SHADER(x)\
	x = NULL; \
	x = new VertexShaderClass;

#define CREATE_PIXEL_SHADER(x) \
	x = NULL; \
	x = new PixelShaderClass;

#define DELETESHADER(x) \
if (x)\
{\
	x->ShutDown(); \
	x = NULL; \
}

#define SAFE_DELETE(x) if(x)\
{delete x; x = nullptr; }

#define SAFE_RELEASE(x) if(x)\
{x->Release(); x = nullptr; }

#define SAFE_CREATE(varPtr,type) varPtr = nullptr;\
	varPtr = new type;

#define MSG_RETURN(var,errormsg)	if(FAILED(var)){\
	MessageBoxA(NULL, errormsg, "error", MB_OK); \
	return E_FAIL; \
}