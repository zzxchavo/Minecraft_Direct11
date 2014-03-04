#pragma once

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <D3D11.h>
#include <D3DX11.h>
#include <d3dx11async.h>
#include <D3Dcompiler.h>
#include "resource.h"
#include "VertexClass.h"
#include "OBBClass.h"
#include "BlockClass.h"
#include "BoneClass.h"
#include "SkyClass.h"
#include "CloudClass.h"
#include "PlayerClass.h"
#include "InputClass.h"
#include "SoundClass.h"
#include "DefinesAndTextures.h"
#include "MapGenerateClass.h"
#include "VertexShaderClass.h"
#include "PixelShaderClass.h"
#include "FrameBuffer.h"
#include "Render2TextureClass.h"
#include "DayTime.h"
#include "Rectangle2DClass.h"
#include <vector>
#include <set>
#include <thread>
#include <mutex>
#include <functional>  

using namespace std;

#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")


struct OutputQueue
{
	int x,y,z;
};

class WorldClass
{
public:
	WorldClass(void);
	~WorldClass(void);

	HRESULT Initialize(ID3D11Device *,ID3D11DeviceContext *,HWND);

	void Inputs();
	void ThreadUpdateMap();
	void OptimizeThread();

	void TurnOnAlphaRendering(ID3D11Device*,ID3D11DeviceContext*);
	void TurnOffAlphaRendering(ID3D11Device*, ID3D11DeviceContext*);

	void TurnOffZBuffer(ID3D11Device*, ID3D11DeviceContext*);
	void TurnOnZBuffer(ID3D11Device*, ID3D11DeviceContext*);

	InputClass * GetInputClass();

	void RenderFrame(ID3D11RenderTargetView *,
		ID3D11DepthStencilView *,
		D3D11_VIEWPORT& ,
		ID3D11Device *,
		ID3D11DeviceContext *);
	SkyClass * GetSkyBox();

	void RenderScene(ID3D11Device *,
		ID3D11DeviceContext *);
private:
	
	VertexShaderClass * basicVS,*skyboxVS,*cloudVS,*texVS;
	PixelShaderClass  * basicPS,*skyboxPS,*cloudPS,*texPS;

	int Chunks;
	int t_playerX,t_playerY,t_playerZ;
	mutex		*t_mutex;		//地图Hash锁
	mutex		*t_output;		//绘图队列锁
	thread		*t_update;		//更新未探索地图
	thread		*t_fastrender;	//更新绘图队列
	bool		updateThreadSwitch;	//更新地图开关
	bool		OptimizeThreadSwitch;//绘图队列开关
	InputClass  *m_input;
	PlayerClass *m_player;
	SkyClass    *m_sky;
	CloudClass  *m_cloud;
	BlockClass	*m_block;
	DayTime		*m_daytime;
	MapGenerateClass   *m_map;
	
//	FrameBuffer colorBuffer,m_DownSampleTexure,m_HorizontalBlurTexture,m_VerticalBlurTexture,m_UpSampleTexure;

	DefinesAndTextures MCTextures;
	SoundClass* m_sound;
	
	vector<iPointClass>RenderQueue;

//Rendering vars.
	ID3D11BlendState* AlphaRenderingEnabled;
	ID3D11BlendState* AlphaRenderingDisabled;
	ID3D11DepthStencilState* DepthStencilEnabled;
	ID3D11DepthStencilState* DepthStencilDisabled;

	Render2TextureClass colorBuffer, horizon, vertical;
	Rectangle2DClass* m_rect,*m_screenRect;

	HWND basic_hwnd;
};