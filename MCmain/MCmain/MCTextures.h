#pragma once

#include<stdio.h>
#include"TextureClass.h"

#define MAX_TEX_NUM   100

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
#define GUI_BORDER_LEFT		  14
#define GUI_BORDER_RIGHT	  15
#define GUI_BORDER_MID		  16

class DefinesAndTextures
{
public:
	static DefinesAndTextures*& GetInstance();
	bool Initialize(ID3D11Device *,ID3D11DeviceContext *);
	TextureClass* GetTextureClass(const int);
	TextureClass* operator [](const int)const ;
private:
	DefinesAndTextures();
	~DefinesAndTextures();
	TextureClass * m_texs[MAX_TEX_NUM];
	static DefinesAndTextures* instance;
};


#define MCTextures (*(DefinesAndTextures::GetInstance()))