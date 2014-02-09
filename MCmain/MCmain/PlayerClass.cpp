#include "PlayerClass.h"


PlayerClass::PlayerClass(void)
{
	m_camera = NULL;
	m_camera = new CameraClass;
	csphere[0].sphere.Center = XMFLOAT3(m_camera->GetX(),
		m_camera->GetY(),
		m_camera->GetZ());
	csphere[0].sphere.Radius = 0.9f;
	csphere[0].Collision = false;
	csphere[1].sphere.Center = XMFLOAT3(m_camera->GetX(),
		m_camera->GetY() + 0.9,
		m_camera->GetZ());
	csphere[1].sphere.Radius = 0.9f;
	csphere[1].Collision = false;
}


PlayerClass::~PlayerClass(void)
{
	if(m_camera)
	{
		delete m_camera;
		m_camera = NULL;
	}
}

void PlayerClass::SetStep(float _step)
{
	m_camera->SetStep(_step);
}

void PlayerClass::SetPosition(ID3D11Device* device,ID3D11DeviceContext* context,float _x,float _y,float _z)
{
	m_camera->SetPosition(device,context,_x,_y,_z);
}

bool PlayerClass::Initialize(ID3D11Device * device,ID3D11DeviceContext * context,MapGenerateClass*map)
{
	m_camera ->Initialize(device,context,map);
	return true;
}

CameraClass * PlayerClass::GetCamera()
{
	return m_camera;
}

float PlayerClass::GetX(){return m_camera->GetX();}
float PlayerClass::GetY(){return m_camera->GetY();}
float PlayerClass::GetZ(){return m_camera->GetZ();}

int PlayerClass::CheckCollision(ID3D11Device* device, ID3D11DeviceContext* context,MapGenerateClass* map)
{
	csphere[0].sphere.Center = XMFLOAT3(m_camera->GetX(),
		m_camera->GetY(),
		m_camera->GetZ());
	csphere[0].sphere.Radius = 0.9f;
	csphere[0].Collision = false;
	csphere[1].sphere.Center = XMFLOAT3(m_camera->GetX(),
		m_camera->GetY() + 0.9,
		m_camera->GetZ());
	csphere[1].sphere.Radius = 0.9f;
	csphere[1].Collision = false;

	XMFLOAT3 _pos((int)m_camera->GetX(), (int)m_camera->GetY() + 1, (int)m_camera->GetZ());
	cbox[0].obox.Center = XMFLOAT3(_pos.x - 1, _pos.y, _pos.z);
	cbox[0].obox.Extents = XMFLOAT3(0.5f,0.5f,0.5f);
	cbox[1].obox.Center = XMFLOAT3(_pos.x - 1, _pos.y - 1, _pos.z);
	cbox[1].obox.Extents = XMFLOAT3(0.5f, 0.5f, 0.5f);
	cbox[2].obox.Center = XMFLOAT3(_pos.x, _pos.y, _pos.z - 1);
	cbox[2].obox.Extents = XMFLOAT3(0.5f, 0.5f, 0.5f);
	cbox[3].obox.Center = XMFLOAT3(_pos.x, _pos.y - 1, _pos.z - 1);
	cbox[3].obox.Extents = XMFLOAT3(0.5f, 0.5f, 0.5f);
	cbox[4].obox.Center = XMFLOAT3(_pos.x + 1, _pos.y, _pos.z);
	cbox[4].obox.Extents = XMFLOAT3(0.5f, 0.5f, 0.5f);
	cbox[5].obox.Center = XMFLOAT3(_pos.x + 1, _pos.y - 1, _pos.z);
	cbox[5].obox.Extents = XMFLOAT3(0.5f, 0.5f, 0.5f);
	cbox[6].obox.Center = XMFLOAT3(_pos.x, _pos.y, _pos.z + 1);
	cbox[6].obox.Extents = XMFLOAT3(0.5f, 0.5f, 0.5f);
	cbox[7].obox.Center = XMFLOAT3(_pos.x, _pos.y - 1, _pos.z + 1);
	cbox[7].obox.Extents = XMFLOAT3(0.5f, 0.5f, 0.5f);
	cbox[8].obox.Center = XMFLOAT3(_pos.x, _pos.y - 2, _pos.z);
	cbox[8].obox.Extents = XMFLOAT3(0.5f, 0.5f, 0.5f);
	cbox[9].obox.Center = XMFLOAT3(_pos.x, _pos.y + 1, _pos.z);
	cbox[9].obox.Extents = XMFLOAT3(0.5f, 0.5f, 0.5f);
	bool flag[3] = {true,true,true};
	for (auto & _Sphere : csphere)
	{
		for (int i = 0 ; i < 10 ; i++)
		{
			if ((*map)[map->BKDHash((int)cbox[i].obox.Center.x,
				(int)cbox[i].obox.Center.z)].
				GetData((int)cbox[i].obox.Center.y)!=TEX_EMPTY)
				_Sphere.Collision = 
					XNA::IntersectSphereOrientedBox(&_Sphere.sphere, &cbox[i].obox);
			if (_Sphere.Collision !=0)
			{
				if (i == 0 || i == 1 || i == 4 || i == 5)
					flag[0] = false;
				if (i == 8 || i == 9)
					flag[1] = false;
				if (i == 2 || i == 3 || i == 6 || i == 7)
					flag[2] = false;
			}
		}
	}
	if (csphere[0].Collision != 0 || csphere[1].Collision != 0)
	{
		if (!flag[0])
			m_camera->SetDX(0.0f);
		if (!flag[1] && m_camera->GetDY() < 0)
			m_camera->SetDY(0.0f);
		if (!flag[2])
			m_camera->SetDZ(0.0f);
//		m_camera->BacktoLastPosition(device,context);
		return true;
	}
	return false;
}