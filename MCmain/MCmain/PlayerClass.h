#pragma once

#include"CameraClass.h"
#include"MapGenerateClass.h"
#include"stdio.h"

#include"xnaCollision.h"

#pragma warning(push)
#pragma warning(disable: 4324)

struct CollisionBox
{
	XNA::OrientedBox obox;
	int Collision;
};

struct CollisionSphere
{
	XNA::Sphere sphere;
	int Collision;
};

#pragma pop

class PlayerClass
{
public:
	PlayerClass(void);
	~PlayerClass(void);

	void SetStep(float);
	void SetPosition(ID3D11Device *, ID3D11DeviceContext *, float, float, float);

	float GetX();
	float GetY();
	float GetZ();

	int CheckCollision(ID3D11Device*, ID3D11DeviceContext*,MapGenerateClass *);

	bool Initialize(ID3D11Device *,ID3D11DeviceContext *,MapGenerateClass*);
	CameraClass * GetCamera();
private:
	CameraClass * m_camera;
	CollisionSphere csphere[2];//人物两格高。上包围球和下包围球
	CollisionBox cbox[10];	//人物临近的方块
};

