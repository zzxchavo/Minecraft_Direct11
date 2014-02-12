#include "OcclusionClass.h"


OcclusionClass::OcclusionClass()
{
}


OcclusionClass::~OcclusionClass()
{
}


HRESULT OcclusionClass::Initialize(ID3D11Device* device,
	ID3D11DeviceContext* context,
	MapGenerateClass* map)
{
	HRESULT hr = S_OK;
	pmap = map;
	return hr;
}


bool OcclusionClass::RenderFrame(float CameraX,float CameraY,float CameraZ,
	int x,int y,int z)
{
	D3DXVECTOR3 pos =
	{
		CameraX,
		CameraY,
		CameraZ,
	};
	if (pos[0] == x && pos[1] == y && pos[2] == z)
		return false;
	if ((*pmap).GetData(x + 1,y, z) == TEX_EMPTY)
		return true;
	if ((*pmap).GetData(x - 1, y, z) == TEX_EMPTY)
		return true;
	if ((*pmap).GetData(x , y + 1, z) == TEX_EMPTY)
		return true;
	if ((*pmap).GetData(x , y - 1, z) == TEX_EMPTY)
		return true;
	if ((*pmap).GetData(x, y, z + 1) == TEX_EMPTY)
		return true;
	if ((*pmap).GetData(x,y, z - 1) == TEX_EMPTY)
		return true;
	return false;
}