#include "WidgetClass.h"

WidgetClass::WidgetClass()
{
	positionx = 0;
	positiony = 0;
	width = 10;
	height = 30;
	SAFE_CREATE(m_left, Rectangle2DClass);
	SAFE_CREATE(m_right, Rectangle2DClass);
	SAFE_CREATE(m_middle, Rectangle2DClass);
}

WidgetClass::WidgetClass(int posx,int posy,int wid,int hei)
{
	positionx = posx;
	positiony = posy;
	width	  = wid;
	height	  = hei;
	if(wid < 10) wid = 10;
	if(height < 30) height = 30;
	SAFE_CREATE(m_left, Rectangle2DClass);
	SAFE_CREATE(m_right, Rectangle2DClass);
	SAFE_CREATE(m_middle, Rectangle2DClass);
}


WidgetClass::~WidgetClass()
{
	SAFE_DELETE(m_left);
	SAFE_DELETE(m_right);
	SAFE_DELETE(m_middle);
}
