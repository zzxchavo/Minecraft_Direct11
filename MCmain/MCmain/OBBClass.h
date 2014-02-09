#pragma once
#include"PointClass.h"
#include"BVClass.h"
#include<D3DX10math.h>

class OBBClass : public BVClass
{
public:
	OBBClass(void);
	~OBBClass(void);
	float TestOBB(OBBClass &);

	void SetPosition(float,float,float);
private:
	D3DXVECTOR3 centre;
	D3DXVECTOR3 axis[3];
	D3DXVECTOR3 extension;
};

