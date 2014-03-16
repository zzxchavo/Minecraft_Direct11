#include "ButtonClass.h"


ButtonClass::ButtonClass(int posx,int posy,int wid,int hei)
{
	positionx = posx;
	positiony = posy;
	width = wid;
	height = hei;
	if (wid < 10) wid = 10;
	if (height < 30) height = 30;
}

void ButtonClass::Render(ID3D11Device* device, ID3D11DeviceContext* context)
{
	UpdateLogic();
//	MCTextures[GUI_BORDER_LEFT]->PSBindTexture(device, context);
	m_left->Render(device, context, positionx, positiony);
//	MCTextures[GUI_BORDER_RIGHT]->PSBindTexture(device, context);
	m_right->Render(device, context, positionx + width + 3, positiony);
//	MCTextures[GUI_BORDER_MID]->PSBindTexture(device, context);
	m_middle->Render(device, context, positionx + 3, positiony);
}

HRESULT ButtonClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
	m_left->Initialize(device,context,screenWidth,screenHeight,3,height);
	m_middle->Initialize(device, context, screenWidth, screenHeight, width, height);
	m_right->Initialize(device, context, screenWidth, screenHeight, 3, height);
	return S_OK;
}

void ButtonClass::SetSize(ID3D11Device* device, ID3D11DeviceContext* context,int wid,int hei)
{
	width = wid;
	height = hei;
	m_middle->UpdateBuffers(context,positionx+3,positiony,width,height);
	m_right->UpdateBuffers(context, positionx + width + 3, positiony, width, height);
}

void ButtonClass::UpdateLogic()
{
	;
}