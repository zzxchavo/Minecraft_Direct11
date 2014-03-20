#include "BlockClass.h"


BlockClass::BlockClass(void)
{
	ZeroMemory(pos,sizeof(pos));
	unsigned int vindex[] = 
	{
		1,2,3,3,4,1,5,6,7,
		7,8,5,1,4,6,6,5,1,
		4,3,7,7,6,4,3,2,8,
		8,7,3,2,1,5,5,8,2
	};
	float ver[][3] =
	{
		{-0.5,-0.5,-0.5},
		{-0.5,0.5,-0.5},
		{0.5,0.5,-0.5},
		{0.5,-0.5,-0.5},
		{-0.5,-0.5,0.5},
		{0.5,-0.5,0.5},
		{0.5,0.5,0.5},
		{-0.5,0.5,0.5},
	};
	unsigned int vnindex[] =
	{
		1,1,1,1,1,1,2,2,2,2,2,2,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,5,5,5,6,6,6,6,6,6
	};
	float nor[][3] =
	{
		{0.0,0.0,-1.0},
		{0.0,0.0,1.0},
		{0.0,-1.0,-0.0},
		{1.0,0.0,0.0},
		{0.0,1.0,0.0},
		{-1.0,0.0,0.0}
	};
	unsigned int vtindex[] =
	{
		1,2,3,3,4,1,4,1,2,2,3,
		4,4,1,2,2,3,4,4,1,2,2,
		3,4,4,1,2,2,3,4,4,1,2,2,3,4
	};
	float tv[][2] =
	{
		{1.0,0.0},
		{1.0,1.0},
		{0.0,1.0},
		{0.0,0.0}
	};
	ZeroMemory(pts,sizeof(pts));
	int m_indexCount = 36;
	indices = new ULONG[m_indexCount];
	for(int i=0;i<m_indexCount;i++)
	{
		memcpy(pts[i].pts._XYZ,ver[vindex[i]-1],sizeof(PointClass));
		memcpy(pts[i].normal  ,nor[vnindex[i]-1],sizeof(float)*3);
		memcpy(pts[i].texture ,tv [vtindex[i]-1],sizeof(float)*2);
		indices[i] = i;
	}
	factors.transparent = 1.0f;
}

BlockClass::~BlockClass(void)
{
	SAFE_RELEASE(m_BlockBuffer);
	SAFE_RELEASE(m_RenderFactors);
	SAFE_RELEASE(m_BlockProperty);
}

HRESULT BlockClass::Initialize(ID3D11Device* device,ID3D11DeviceContext* context)
{
	HRESULT hr = S_OK;

	scaling[0] = 1.0f;
	scaling[1] = 1.0f;
	scaling[2] = 1.0f;

	D3D11_BUFFER_DESC bd;
	
	ZeroMemory(&bd,sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VertexClass) * 36;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA sb;
	ZeroMemory(&sb,sizeof(sb));
	sb.pSysMem = &pts[0].pts._XYZ[0];
	hr = device->CreateBuffer(&bd,&sb,&m_BlockBuffer);
	if(FAILED(hr))
	{
		return hr;
	}
	ZeroMemory(&bd,sizeof(bd));
	bd.ByteWidth = sizeof(Propertys);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.Usage =  D3D11_USAGE_DYNAMIC;
	hr = device->CreateBuffer(&bd,NULL,&m_BlockProperty);
	if(FAILED(hr))
	{
		return hr;
	}
	ZeroMemory(&bd,sizeof(bd));
	bd.ByteWidth = sizeof(Factors);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.Usage =  D3D11_USAGE_DYNAMIC;
	hr = device->CreateBuffer(&bd,NULL,&m_RenderFactors);
	if(FAILED(hr))
	{
		return hr;
	}
	rotation[0] = 0.0f;
	rotation[1] = 0.0f;
	rotation[2] = 0.0f;
	D3DXMatrixIdentity(&propertys.RotationMatrix);

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(ULONG)*36;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	hr = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(hr))
		return hr;

	Propertys *dptr;
	Factors    *dptr2;
	Propertys Combine;
	D3D11_MAPPED_SUBRESOURCE myresource;
	D3DXMatrixTranspose(&Combine.TranslationMatrix, &propertys.TranslationMatrix);
	D3DXMatrixTranspose(&Combine.ScalingMatrix, &propertys.ScalingMatrix);
	D3DXMatrixTranspose(&Combine.RotationMatrix, &propertys.RotationMatrix);
	hr = context->Map(m_BlockProperty, 0, D3D11_MAP_WRITE_DISCARD, 0, &myresource);
	dptr = (Propertys *)myresource.pData;
	*dptr = Combine;
	context->Unmap(m_BlockProperty, 0);
	context->VSSetConstantBuffers(2, 1, &m_BlockProperty);

	D3D11_MAPPED_SUBRESOURCE renderFac;
	//设置透明度等等渲染参数
	hr = context->Map(m_RenderFactors, 0, D3D11_MAP_WRITE_DISCARD, 0, &renderFac);
	dptr2 = (Factors *)renderFac.pData;
	*dptr2 = factors;
	context->Unmap(m_RenderFactors, 0);
	context->PSSetConstantBuffers(0, 1, &m_RenderFactors);

	return hr;
}

void BlockClass::SetTransparency(ID3D11Device *device,ID3D11DeviceContext * context,float trans)
{
	HRESULT hr;
	factors.transparent = trans;
	return ;
}

HRESULT BlockClass::Shutdown()
{
	SAFE_DELETE(indices);
	return S_OK;
}
void BlockClass::Render(ID3D11Device* device,ID3D11DeviceContext* context)
{
	HRESULT hr;
	UINT stride = sizeof(VertexClass);
	UINT offset = 0;
	
	Propertys *dptr;
	Factors    *dptr2;
	Propertys Combine;

	D3D11_MAPPED_SUBRESOURCE myresource;
	D3DXMatrixTranspose(&Combine.TranslationMatrix,&propertys.TranslationMatrix);
	D3DXMatrixTranspose(&Combine.ScalingMatrix, &propertys.ScalingMatrix);
	D3DXMatrixTranspose(&Combine.RotationMatrix, &propertys.RotationMatrix);
	hr = context->Map(m_BlockProperty, 0, D3D11_MAP_WRITE_DISCARD, 0, &myresource);
	dptr = (Propertys *) myresource.pData;
	*dptr = Combine;
	context->Unmap(m_BlockProperty, 0);
	context->VSSetConstantBuffers(2, 1, &m_BlockProperty);

	D3D11_MAPPED_SUBRESOURCE renderFac;
	//设置透明度等等渲染参数
	hr = context->Map(m_RenderFactors, 0, D3D11_MAP_WRITE_DISCARD, 0, &renderFac);
	dptr2 = (Factors *)renderFac.pData;
	*dptr2 = factors;
	context->Unmap(m_RenderFactors, 0);
	context->PSSetConstantBuffers(0, 1, &m_RenderFactors);

	context ->IASetVertexBuffers(0,1,&m_BlockBuffer,&stride,&offset);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->DrawIndexed(36, 0, 0);
}

void BlockClass::SetPosition(float _x,float _y,float _z,ID3D11Device* device,ID3D11DeviceContext* context)
{
	HRESULT hr;
	pos[0] = _x;
	pos[1] = _y;
	pos[2] = _z;
	D3DXMatrixTranslation(&propertys.TranslationMatrix,pos[0],pos[1],pos[2]);
	return ;
}


void BlockClass::SetScaling(float _x,float _y,float _z,ID3D11Device* device,ID3D11DeviceContext* context)
{
	HRESULT hr;
	scaling[0] = _x;
	scaling[1] = _y;
	scaling[2] = _z;
	D3DXMatrixScaling(&propertys.ScalingMatrix,scaling[0],scaling[1],scaling[2]);
	return ;
}

void BlockClass::SetRotation(float yaw,float pitch,float roll,ID3D11Device* device,ID3D11DeviceContext* context)
{
	HRESULT hr;
	rotation[0] = yaw;
	rotation[1] = pitch;
	rotation[2] = roll;
	D3DXMatrixRotationYawPitchRoll(&propertys.RotationMatrix,
		rotation[0],rotation[1],rotation[2]);
	return ;
}