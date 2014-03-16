#pragma once
<<<<<<< HEAD
#include<D3D11.h>
#include<D3DX11.h>
=======
>>>>>>> 63f768f2ac038be9c0895581b94580b55bbf5bd0
const int screenWidth = 800, screenHeight = 600;

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