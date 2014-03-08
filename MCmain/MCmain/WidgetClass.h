#pragma once
#include"Rectangle2DClass.h"
#include"Defines.h"

class WidgetClass
{
public:
	WidgetClass();
	WidgetClass(int posx,int posy,int wid,int hei);
	~WidgetClass();
	virtual HRESULT Initialize(ID3D11Device* device,ID3D11DeviceContext* context) = 0;
	virtual void Render(ID3D11Device* device,ID3D11DeviceContext* context) = 0;
protected:
	virtual void UpdateLogic() = 0; //invoke this section inside 'Render' function
	int positionx, positiony, width, height;
	Rectangle2DClass* m_left;
	Rectangle2DClass* m_right;
	Rectangle2DClass* m_middle;
};