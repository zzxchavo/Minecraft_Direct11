#include "CameraClass.h"
CameraClass::CameraClass(void)
{
	MatrixBuffer = NULL;
	m_frustum = NULL;
	m_occlusion = NULL;
	Fov = 45.0f;
	NearDist = 0.01f;
	FarDist  = 1000.0f;
	D3DXMatrixIdentity(&mat.WorldMatrix);
	D3DXMatrixIdentity(&mat.ProjectionMatrix);
	D3DXMatrixIdentity(&mat.ViewMatrix);
	D3DXMatrixPerspectiveFovLH(&mat.ProjectionMatrix,Fov,4.0/3.0,NearDist,FarDist) ;
	D3DXVECTOR3 Eye(0.0f,1.0f,-2.0f),
		At(0.0f,0.0f,1.0f),dup(0.0f,1.0f,0.0f);
	Up = dup;
	D3DXMatrixLookAtLH(&mat.ViewMatrix,&Eye,&At,&Up);
	m_frustum = new FrustumClass;
	m_occlusion = new OcclusionClass;
	step = 1.0f;
	return ;
}

CameraClass::~CameraClass(void)
{
	if(MatrixBuffer)
	{
		MatrixBuffer->Release();
		MatrixBuffer = NULL;
	}
	if(m_frustum)
	{
		delete m_frustum;
		m_frustum = NULL;
	}
	if (m_occlusion)
	{
		delete m_occlusion;
		m_occlusion = NULL;
	}
	return;
}

void CameraClass::AddRotationX(float _rx)
{
	m_rotationX += _rx;
	if(m_rotationX>100) m_rotationX = 100;
	if(m_rotationX<-100) m_rotationX = -100;
}

void CameraClass::AddRotationY(float _ry)
{
	m_rotationY += _ry;
	if(m_rotationY>360.0)
		m_rotationY -= 360.0;
	if(m_rotationY<-360.0)
		m_rotationY += 360.0;
}

void CameraClass::AddRotationZ(float _rz)
{
	m_rotationZ += _rz;
	if(m_rotationZ>360.0)
		m_rotationZ -= 360.0;
	if(m_rotationZ<-360.0)
		m_rotationZ += 360.0;
}
bool CameraClass::Initialize(ID3D11Device * device,ID3D11DeviceContext* context,MapGenerateClass* m_map)
{
	//////所有MatrixBuffer都占用的是b1管线
	//Create MatrixBuffers Here.
	HRESULT hr;
	
	pdevice = (unique_ptr<ID3D11Device*>)&device;
	pcontext = (unique_ptr<ID3D11DeviceContext*>)&context;
	
	m_rotationX = 0;
	m_rotationY = 0;
	m_rotationZ = 0;
	ZeroMemory(&bd,sizeof(bd));
	bd.ByteWidth = sizeof(matrix);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.Usage =  D3D11_USAGE_DYNAMIC;
	D3DXMatrixIdentity(&mat.WorldMatrix);
	D3DXMatrixIdentity(&mat.ProjectionMatrix);
	D3DXMatrixIdentity(&mat.ViewMatrix);



	float pitch = m_rotationX * 0.0174532925f;
	float yaw   = m_rotationY * 0.0174532925f;
	float roll  = m_rotationZ * 0.0174532925f;

	D3DXMATRIX rotationMatrix;
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);
	mat.WorldMatrix *= rotationMatrix;

	D3DXMatrixPerspectiveFovLH(&mat.ProjectionMatrix,Fov,16.0/9.0,NearDist,FarDist) ;
	D3DXVECTOR3 Eye(0.0f,0.0f,0.0f),
		At(0.0f,0.0f,1.0f);
	D3DXMatrixLookAtLH(&mat.ViewMatrix,&Eye,&At,&Up);
	device->CreateBuffer(&bd,0,&MatrixBuffer);

	matrix *dptr;
	matrix Combine;
	D3D11_MAPPED_SUBRESOURCE myresource;
	D3DXMatrixTranspose(&Combine.WorldMatrix,&mat.WorldMatrix);
	D3DXMatrixTranspose(&Combine.ProjectionMatrix,&mat.ProjectionMatrix);
	D3DXMatrixTranspose(&Combine.ViewMatrix,&mat.ViewMatrix);

	hr = context->Map(MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &myresource);
	dptr = (matrix *) myresource.pData;
	*dptr = Combine;
	context->Unmap(MatrixBuffer, 0);
	context->VSSetConstantBuffers(1, 1, &MatrixBuffer);
	
	
	m_frustum->ConstructFrustum(FarDist,mat.ProjectionMatrix,mat.ViewMatrix);
	m_occlusion->Initialize(device, context, m_map);
	return true;
}

void CameraClass::UpdateCamera(ID3D11Device * device,ID3D11DeviceContext* context)
{
	HRESULT hr;
	D3DXMatrixIdentity(&mat.WorldMatrix);
	D3DXMatrixIdentity(&mat.ProjectionMatrix);
	D3DXMatrixIdentity(&mat.ViewMatrix);
	D3DXMatrixPerspectiveFovLH(&mat.ProjectionMatrix,Fov,4.0/3.0,NearDist,FarDist) ;
	if(_pos[0] != 0 || _pos[1] != 0 ||_pos[2] != 0)
	{
		LastPos = _pos;
		D3DXVec3Normalize(&dvec,&dvec);
		_pos += (dvec*step);
	}

	D3DXVECTOR3 Eye(_pos[0],_pos[1],_pos[2]),
		At(0.0f,0.0f,1.0f),
		left(-1.0f,0.0f,0.0f);
	Up[0] = 0.0f;
	Up[1] = 1.0f;
	Up[2] = 0.0f;
	
	float pitch = m_rotationX * 0.0174532925f;
	float yaw   = m_rotationY * 0.0174532925f;
	float roll  = m_rotationZ * 0.0174532925f;

	D3DXMATRIX rotationMatrix,positionMatrix;
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	D3DXVec3TransformCoord(&At, &At, &rotationMatrix);
	D3DXVec3TransformCoord(&Up, &Up, &rotationMatrix);
	D3DXVec3TransformCoord(&Left, &left, &rotationMatrix);
	Heading = At;
	At += Eye;

	D3DXMatrixLookAtLH(&mat.ViewMatrix,&Eye,&At,&Up);

	matrix *dptr;
	matrix Combine;

	D3D11_MAPPED_SUBRESOURCE myresource;
	D3DXMatrixTranspose(&Combine.WorldMatrix,&mat.WorldMatrix);
	D3DXMatrixTranspose(&Combine.ProjectionMatrix,&mat.ProjectionMatrix);
	D3DXMatrixTranspose(&Combine.ViewMatrix,&mat.ViewMatrix);

	hr = context->Map(MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &myresource);
	dptr = (matrix *) myresource.pData;
	*dptr = Combine;
	context->Unmap(MatrixBuffer, 0);
	context->VSSetConstantBuffers(1, 1, &MatrixBuffer);
	m_frustum->ConstructFrustum(FarDist,mat.ProjectionMatrix,mat.ViewMatrix);
	ZeroMemory(&dvec,sizeof(dvec));
}
void CameraClass::SetFov(float _Fov)
{
	Fov = _Fov;
	D3DXMatrixPerspectiveFovLH(&mat.ProjectionMatrix,Fov,16.0/9.0,NearDist,FarDist) ;
}
void CameraClass::SetFrustum(float _near,float _far)
{
	NearDist = _near;
	FarDist  = _far;
	D3DXMatrixPerspectiveFovLH(&mat.ProjectionMatrix,Fov,16.0/9.0,NearDist,FarDist) ;
}

float &CameraClass::GetX(){return _pos[0];}
float &CameraClass::GetY(){return _pos[1];}
float &CameraClass::GetZ(){return _pos[2];}

void CameraClass::SetPosition(ID3D11Device* device,ID3D11DeviceContext* context,float _x,float _y,float _z)
{
	_pos[0] = _x;
	_pos[1] = _y;
	_pos[2] = _z;
	LastPos = _pos;
	D3DXVECTOR3 Eye(_pos[0], _pos[1], _pos[2]),
		At(0.0f, 0.0f, 1.0f),
		left(-1.0f, 0.0f, 0.0f);
	Up[0] = 0.0f;
	Up[1] = 1.0f;
	Up[2] = 0.0f;

	float pitch = m_rotationX * 0.0174532925f;
	float yaw = m_rotationY * 0.0174532925f;
	float roll = m_rotationZ * 0.0174532925f;

	D3DXMATRIX rotationMatrix, positionMatrix;
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	D3DXVec3TransformCoord(&At, &At, &rotationMatrix);
	D3DXVec3TransformCoord(&Up, &Up, &rotationMatrix);
	D3DXVec3TransformCoord(&Left, &left, &rotationMatrix);
	Heading = At;
	At += Eye;

	D3DXMatrixLookAtLH(&mat.ViewMatrix, &Eye, &At, &Up);

	matrix *dptr;
	matrix Combine;

	D3D11_MAPPED_SUBRESOURCE myresource;
	D3DXMatrixTranspose(&Combine.WorldMatrix, &mat.WorldMatrix);
	D3DXMatrixTranspose(&Combine.ProjectionMatrix, &mat.ProjectionMatrix);
	D3DXMatrixTranspose(&Combine.ViewMatrix, &mat.ViewMatrix);

	context->Map(MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &myresource);
	dptr = (matrix *)myresource.pData;
	*dptr = Combine;
	context->Unmap(MatrixBuffer, 0);
	context->VSSetConstantBuffers(1, 1, &MatrixBuffer);
	m_frustum->ConstructFrustum(FarDist, mat.ProjectionMatrix, mat.ViewMatrix);
	ZeroMemory(&dvec, sizeof(dvec));
}

void CameraClass::MoveForward()
{
	dvec += Heading;
}

void CameraClass::MoveBackward()
{
	dvec -= Heading;
}

void CameraClass::MoveLeft()
{
	dvec += Left;
}

void CameraClass::MoveRight()
{
	dvec -= Left;
}

void CameraClass::SetStep(float _step)
{
	step = _step;
}

void CameraClass::FlyUp()
{
	D3DXVECTOR3 _tup(0.0f,1.0f,0.0f);
	dvec += _tup;
}

void CameraClass::DiveDown()
{
	D3DXVECTOR3 _tup(0.0f,1.0f,0.0f);
	dvec -= _tup;
}

bool CameraClass::CheckBox(float CentreX,float CentreY,float CentreZ,float Size)
{
	return m_frustum->CheckCube(CentreX, CentreY, CentreZ, Size)&&
		m_occlusion->RenderFrame(_pos[0],_pos[1],_pos[2],CentreX,CentreY,CentreZ);
}

void CameraClass::BacktoLastPosition(ID3D11Device* device, ID3D11DeviceContext* context)
{
	_pos = LastPos;
	D3DXVECTOR3 Eye(_pos[0], _pos[1], _pos[2]),
		At(0.0f, 0.0f, 1.0f),
		left(-1.0f, 0.0f, 0.0f);
	Up[0] = 0.0f;
	Up[1] = 1.0f;
	Up[2] = 0.0f;

	float pitch = m_rotationX * 0.0174532925f;
	float yaw = m_rotationY * 0.0174532925f;
	float roll = m_rotationZ * 0.0174532925f;

	D3DXMATRIX rotationMatrix, positionMatrix;
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	D3DXVec3TransformCoord(&At, &At, &rotationMatrix);
	D3DXVec3TransformCoord(&Up, &Up, &rotationMatrix);
	D3DXVec3TransformCoord(&Left, &left, &rotationMatrix);
	Heading = At;
	At += Eye;

	D3DXMatrixLookAtLH(&mat.ViewMatrix, &Eye, &At, &Up);

	matrix *dptr;
	matrix Combine;

	D3D11_MAPPED_SUBRESOURCE myresource;
	D3DXMatrixTranspose(&Combine.WorldMatrix, &mat.WorldMatrix);
	D3DXMatrixTranspose(&Combine.ProjectionMatrix, &mat.ProjectionMatrix);
	D3DXMatrixTranspose(&Combine.ViewMatrix, &mat.ViewMatrix);

	(context)->Map(MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &myresource);
	dptr = (matrix *)myresource.pData;
	*dptr = Combine;
	(context)->Unmap(MatrixBuffer, 0);
	(context)->VSSetConstantBuffers(1, 1, &MatrixBuffer);
	m_frustum->ConstructFrustum(FarDist, mat.ProjectionMatrix, mat.ViewMatrix);
	ZeroMemory(&dvec, sizeof(dvec));
}

void CameraClass::SetDX(float val)
{
	dvec.x = val;
}

void CameraClass::SetDY(float val)
{
	dvec.y = val;
}

void CameraClass::SetDZ(float val)
{
	dvec.z = val;
}

float& CameraClass::GetDX()
{
	return dvec[0];
}

float& CameraClass::GetDY()
{
	return dvec[1];
}

float& CameraClass::GetDZ()
{
	return dvec[2];
}