#pragma once
#include<D3D11.h>
#include<D3DX11.h>
#include<D3DX10math.h>
#include"CameraClass.h"
#include"InputClass.h"

class PickingClass
{
public:
	PickingClass();
	~PickingClass();
	HRESULT UpdateRay(InputClass* input,CameraClass* camera);
	D3DXVECTOR3& GetDirection();
	D3DXVECTOR3& GetOrigin();
private:
	D3DXVECTOR3 direction;
	D3DXVECTOR3 origin;
};
