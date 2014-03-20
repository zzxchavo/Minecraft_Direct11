#include "PickingClass.h"


PickingClass::PickingClass()
{
}


PickingClass::~PickingClass()
{
}

HRESULT PickingClass::UpdateRay(InputClass* input,CameraClass* camera)
{
	HRESULT hr = S_OK;
	float mousex = input->GetMouseX(),mousey = input->GetMouseY();
	D3DXMATRIX projection = camera->GetProjectionMatrix();
	D3DXMATRIX view = camera->GetViewMatrix();
	D3DXMATRIX world = camera->GetWorldMatrix();
	D3DXMATRIX inverse;
	D3DXMatrixInverse(&inverse, NULL, &view);

	mousex = (2.0f * mousex) / screenWidth - 1.0f;
	mousey = (2.0f * mousey) / screenHeight - 1.0f;
	mousex /= projection._11;
	mousey /= -projection._22;
	
	direction.x = (mousex*inverse._11) + (mousey*inverse._21) + inverse._31;
	direction.y = (mousex*inverse._12) + (mousey*inverse._22) + inverse._32;
	direction.z = (mousex*inverse._13) + (mousey*inverse._23) + inverse._33;

	D3DXVec3Normalize(&direction, &direction);
	origin = { camera->GetX() ,camera->GetY(),camera->GetZ()};

	return hr;
}

D3DXVECTOR3& PickingClass::GetDirection()
{
	return direction;
}

D3DXVECTOR3& PickingClass::GetOrigin()
{
	return origin;
}