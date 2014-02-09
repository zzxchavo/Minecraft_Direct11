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
	if ((*pmap)[BKDHASH(x + 1, z)].GetData(y) == TEX_EMPTY)
		return true;
	if ((*pmap)[BKDHASH(x - 1, z)].GetData(y) == TEX_EMPTY)
		return true;
	if ((*pmap)[BKDHASH(x, z)].GetData(y + 1) == TEX_EMPTY)
		return true;
	if ((*pmap)[BKDHASH(x, z)].GetData(y - 1) == TEX_EMPTY)
		return true;
	if ((*pmap)[BKDHASH(x, z + 1)].GetData(y) == TEX_EMPTY)
		return true;
	if ((*pmap)[BKDHASH(x, z - 1)].GetData(y) == TEX_EMPTY)
		return true;
	return false;
}