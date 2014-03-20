#include "BoneClass.h"


BoneClass::BoneClass()
{
	name = "";
	parent = nullptr;
	SonNum = 0;
	rotation[0] = 0.0f;
	rotation[1] = 0.0f;
	rotation[2] = 0.0f;
	position[0] = 0.0f;
	position[1] = 0.0f;
	position[2] = 0.0f;
	D3DXMatrixIdentity(&tramat);
	D3DXMatrixIdentity(&rotmat);
	D3DXMatrixIdentity(&ptramat);
	D3DXMatrixIdentity(&protmat);
}

BoneClass::~BoneClass()
{
	;
}

bool BoneClass::isRoot()
{
	return parent == nullptr;
}

bool BoneClass::isLeaf()
{
	return SonNum == 0;
}
void BoneClass::SetName(string str)
{
	name = str;
}

void BoneClass::SetPosition(float x, float y, float z)
{
	if (isRoot())
	{
		position[0] = x;
		position[1] = y;
		position[2] = z;
	}
	else
	{
		position[0] = x - (parent->position[0]);
		position[1] = y - (parent->position[1]);
		position[2] = z - (parent->position[2]);
	}
	D3DXMatrixTranslation(&tramat, position[0], position[1], position[2]);
	if(!isRoot())	ptramat = tramat * parent->protmat * parent->ptramat;
	else ptramat = tramat;
	return;
}

bool BoneClass::SetParent(BoneClass* _parent)//return true means succeed,else fail
{
	if(_parent == nullptr) return false;
	parent = _parent;
}

bool BoneClass::BindWithAsParent(BoneClass* sonbone)
{
	if (SonNum >= 10) return false;
	sons[SonNum++] = sonbone;
	sonbone->SetParent(this);
	sons[SonNum - 1]->position -= position;
	D3DXMatrixTranslation(&tramat, position[0], position[1], position[2]);
	if(!isRoot()) ptramat = tramat * parent->protmat * parent->ptramat;
	else ptramat = tramat;
	return true;
}

int BoneClass::GetSonNum()
{
	return SonNum;
}

void BoneClass::SetYawPitchRoll(float yaw,float pitch,float roll)
{
	rotation = {yaw,pitch,roll};
	D3DXMatrixRotationYawPitchRoll(&rotmat,rotation[0],rotation[1],rotation[2]);
//	if (!isRoot()) protmat = rotmat * parent->rotmat;
//	else 
		protmat = rotmat;
	return ;
}

D3DXVECTOR3 BoneClass::GetPosition()
{
	if (isRoot()) return position;
	D3DXMATRIX mat,tem;
//	D3DXVECTOR3 _r = parent->GetPosition();
//	D3DXMatrixTranslation(&tem, _r[0], _r[1], _r[2]);
	mat = tramat * parent->protmat * parent->ptramat;
	ptramat = mat;
	D3DXVECTOR3 ret;
	ret[0] = mat._41;
	ret[1] = mat._42;
	ret[2] = mat._43;
	return ret;
}


void DrawLine(BlockClass* block, ID3D11Device* device, ID3D11DeviceContext* context,
	float var, float var0, float var1, float var2, float var3, float var4)
{
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	float midx = (var + var2) / 2.0f;
	float midy = (var0 + var3) / 2.0f;
	float midz = (var1 + var4) / 2.0f;
	float xdist = abs(var - var2);
	float ydist = abs(var0 - var3);
	float zdist = abs(var1 - var4);
	float length = sqrt((xdist*xdist) + (ydist*ydist) + (zdist*zdist));
	float xzlength = sqrt((xdist*xdist) + (zdist*zdist));
	float yaw = acos(zdist / xzlength),
		pitch = acos(xzlength / length),
		roll = 0.0f;
	if (var3 - var0 < 0) pitch = -pitch;
	if (((var2 - var < 0) && (var4 - var1 > 0)) ||
		((var2 - var > 0) && (var4 - var1 < 0)))
		yaw = -yaw;
	block->SetPosition(midx,midy,midz,device,context);
	block->SetScaling(0.01f, 0.01f, length, device, context);
	block->SetRotation(yaw,pitch,roll,device,context);
	block->Render(device,context);
	context->Draw(36,0);
	block->SetRotation(0.0f,0.0f,0.0f,device,context);
	block->SetScaling(1.0f, 1.0f, 1.0f, device, context);	
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}