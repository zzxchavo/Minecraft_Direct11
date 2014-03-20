#pragma once
#include<D3D11.h>
#include<D3DX11.h>
#include<D3DX10math.h>
#include<math.h>
#include<stdio.h>
#include<memory>
#include"FrustumClass.h"
#include"OcclusionClass.h"
#include "MapGenerateClass.h"
class CameraClass
{
public:
	CameraClass(void);
	~CameraClass(void);
	
	bool Initialize(ID3D11Device*,ID3D11DeviceContext*,MapGenerateClass*);

	void UpdateCamera(ID3D11Device*,ID3D11DeviceContext*);

	void SetFov(float);
	void SetFrustum(float,float);
	void SetPosition(ID3D11Device*,ID3D11DeviceContext*,float,float,float);
	void SetStep(float);

	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();
	void FlyUp();
	void DiveDown();

	bool CheckBox(float,float,float,float);

	float& GetX();
	float& GetY();
	float& GetZ();

	float& GetDX();
	float& GetDY();
	float& GetDZ();

	void AddRotationX(float);
	void AddRotationY(float);
	void AddRotationZ(float);

	void BacktoLastPosition(ID3D11Device*,ID3D11DeviceContext*);

	void SetDX(float);
	void SetDY(float);
	void SetDZ(float);

	D3DXMATRIX& GetProjectionMatrix();
	D3DXMATRIX& GetViewMatrix();
	D3DXMATRIX& GetWorldMatrix();

private:
	float m_rotationX;
	float m_rotationY;
	float m_rotationZ;
	float Fov;
	float NearDist;
	float FarDist;
	float step;
	D3DXVECTOR3 LastPos;
	D3DXVECTOR3 _pos;
	D3DXVECTOR3 Up;
	D3DXVECTOR3 Heading;
	D3DXVECTOR3 Left;
	D3DXVECTOR3 dvec;
	struct matrix{
		D3DXMATRIX ProjectionMatrix;
		D3DXMATRIX ViewMatrix;
		D3DXMATRIX WorldMatrix;
	}mat;
	ID3D11Buffer* MatrixBuffer;
	D3D11_BUFFER_DESC bd;
	FrustumClass* m_frustum;
	OcclusionClass * m_occlusion;
};

