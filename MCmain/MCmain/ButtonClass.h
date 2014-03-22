#pragma once
#include"WidgetClass.h"
#include"MCTextures.h"
class ButtonClass : public WidgetClass
{
public:
	ButtonClass(int,int,int,int);
	HRESULT Initialize(ID3D11Device* devicce,ID3D11DeviceContext* context);
//	void UpdateLogic();
	void Render(ID3D11Device* device, ID3D11DeviceContext* context);
	void SetSize(ID3D11Device* device,ID3D11DeviceContext* context,int wid,int hei);
private:
	void UpdateLogic();
};
