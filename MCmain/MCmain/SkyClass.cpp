#include "SkyClass.h"


SkyClass::SkyClass(float Size)
{
	m_BlockBuffer = NULL;
	unsigned int vindex[] = 
	{
		1,2,3,4,3,2,5,6,7,
		8,7,6,4,2,5,6,5,2,
		3,4,7,5,7,4,1,3,8,
		7,8,3,2,1,6,8,6,1
	};
	float h_sizer = Size/2.0f;
	float ver[][3] =
	{
		{-h_sizer, h_sizer,-h_sizer},
		{-h_sizer,-h_sizer,-h_sizer},
		{ h_sizer, h_sizer,-h_sizer},
		{ h_sizer,-h_sizer,-h_sizer},
		{ h_sizer,-h_sizer, h_sizer},
		{-h_sizer,-h_sizer, h_sizer},
		{ h_sizer, h_sizer, h_sizer},
		{-h_sizer, h_sizer, h_sizer},
	};
	unsigned int vnindex[] =
	{
		1,1,1,1,1,1,2,2,2,2,2,2,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,5,5,5,6,6,6,6,6,6
	};
	float nor[][3] =
	{
		{0.0,0.0,1.0},
		{0.0,0.0,-1.0},
		{0.0,1.0,0.0},
		{-1.0,0.0,0.0},
		{0.0,-1.0,0.0},
		{1.0,0.0,0.0}
	};
	unsigned int vtindex[] =
	{
		1,2,3,4,3,2,
		2,4,1,3,1,4,
		2,4,1,3,1,4,
		2,4,1,3,1,4,
		2,4,1,3,1,4,
		2,4,1,3,1,4
	};
	float tv[][2] =
	{
		{1.0,1.0},
		{1.0,0.0},
		{0.0,1.0},
		{0.0,0.0}
	};
	ZeroMemory(pts,sizeof(pts));
	for(int i=0;i<36;i++)
	{
		memcpy(pts[i].pts._XYZ,ver[vindex[i]-1],sizeof(PointClass));
		memcpy(pts[i].normal  ,nor[vnindex[i]-1],sizeof(float)*3);
		memcpy(pts[i].texture ,tv [vtindex[i]-1],sizeof(float)*2);
	}
}

SkyClass::~SkyClass(void)
{
	if(m_BlockBuffer)
	{
		m_BlockBuffer ->Release();
		m_BlockBuffer = NULL;
	}
}
