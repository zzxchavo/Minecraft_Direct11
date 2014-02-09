#include "OBBClass.h"


OBBClass::OBBClass(void)
{
}


OBBClass::~OBBClass(void)
{
}

float dot(D3DXVECTOR3 & a,D3DXVECTOR3 & b)
{
	return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}
float OBBClass::TestOBB(OBBClass & other)
{
	float ra,rb;
	D3DXMATRIX r,absR;
	D3DXMatrixIdentity(&r);
	D3DXMatrixIdentity(&absR);
	D3DXVECTOR3 tt = other.centre - centre;
	D3DXVECTOR3 t(dot(tt,axis[0]),dot(tt,axis[1]),dot(tt,axis[2]));
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
		{
			r[i*4+j] = dot(axis[i],other.axis[j]);
		}
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
			absR[i*4+j] = abs(r[i*4+j])+0.000000001;
	for(int i=0;i<3;i++)
	{
		ra = extension[i];
		rb = other.extension[0]*absR[i*4] + 
			other.extension[1]*absR[i*4+1] + 
			other.extension[2]*absR[i*4+2];
		if(abs(t[i])>ra+rb)
			return 0;
	}
	for(int i=0;i<3;i++)
	{
		ra = extension[0]*absR[i] +
			extension[1]*absR[4+i]
		+ extension[2] * absR[8+i];
		rb = other.extension[i];
		if(abs(t[0]*r[i] + t[1]*r[4+i]+t[2]*r[8+i])>(ra+rb))
			return 0;
	}
	//L=A0 * B0
	ra = extension[1]*absR[8]+extension[2]*absR[4];
	rb = other.extension[1]*absR[2] + other.extension[2]*absR[1];
	if(abs(t[2]*r[4] - t[1]*r[8])>ra+rb) 
		return 0;
	//L = A0 * B1
	ra = extension[1]*absR[9] + extension[2]*absR[5];
	rb = other.extension[0] * absR[2] + other.extension[2]*absR[0];
	if(abs(t[2]*r[5] - t[1]*r[9])>ra+rb)
		return 0;
	//L = A0 * B2
	ra = extension[1]*absR[10] + extension[2]*absR[6];
	rb = other.extension[0] * absR[1] + other.extension[1]*absR[0];
	if(abs(t[2]*r[6] - t[1]*r[10])>ra+rb)
		return 0;
	//L = A1 * B0
	ra = extension[0]*absR[8] + extension[2]*absR[0];
	rb = other.extension[1] * absR[6] + other.extension[2]*absR[5];
	if(abs(t[0]*r[8] - t[2]*r[0])>ra+rb)
		return 0;
	//L = A1 * B1
	ra = extension[0]*absR[9] + extension[2]*absR[1];
	rb = other.extension[0] * absR[6] + other.extension[2]*absR[4];
	if(abs(t[0]*r[9] - t[2]*r[1])>ra+rb)
		return 0;
	//L = A1 * B2
	ra = extension[0]*absR[10] + extension[2]*absR[2];
	rb = other.extension[0] * absR[5] + other.extension[1]*absR[4];
	if(abs(t[0]*r[10] - t[2]*r[2])>ra+rb)
		return 0;
	//L = A2 * B0
	ra = extension[0]*absR[4] + extension[1]*absR[0];
	rb = other.extension[1] * absR[10] + other.extension[2]*absR[9];
	if(abs(t[1]*r[0] - t[0]*r[4])>ra+rb)
		return 0;
	//L = A2 * B1
	ra = extension[0]*absR[5] + extension[1]*absR[1];
	rb = other.extension[0] * absR[10] + other.extension[2]*absR[8];
	if(abs(t[0]*r[1] - t[0]*r[5])>ra+rb)
		return 0;
	//L = A2 * B2
	ra = extension[0]*absR[6] + extension[1]*absR[2];
	rb = other.extension[0] * absR[9] + other.extension[1]*absR[8];
	if(abs(t[1]*r[2] - t[0]*r[6])>ra+rb)
		return 0;
	return 1;
}