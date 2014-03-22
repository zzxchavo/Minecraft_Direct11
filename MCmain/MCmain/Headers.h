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
#include "GUI.h"
#include "ShaderManager.h"
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
#include "Defines.h"
#include "MCTextures.h"
#include "MapGenerateClass.h"
#include "VertexShaderClass.h"
#include "PixelShaderClass.h"
#include "Render2TextureClass.h"
#include "DayTime.h"
#include "DrawLine.hpp"
#include "Rectangle2DClass.h"
#include "EffectManager.h"
#include "PickingClass.h"
#include "ModelClass.h"
#include <vector>
#include <set>
#include <thread>
#include <mutex>
#include <functional> 
//#include <ft2build.h>
using namespace std;


#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")
//#pragma comment(lib, "freetype253.lib")
//#pragma comment(lib, "freetype253_D.lib")