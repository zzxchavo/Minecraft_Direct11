#include "Rectangle2DClass.h"

Rectangle2DClass::Rectangle2DClass()
{
}

Rectangle2DClass::~Rectangle2DClass(void)
{
	SAFE_RELEASE(m_BlockBuffer);
	SAFE_RELEASE(m_RenderFactors);
	SAFE_RELEASE(m_BlockProperty);
	SAFE_RELEASE(m_indexBuffer);
}

bool Rectangle2DClass::Initialize(ID3D11Device * device, ID3D11DeviceContext* context, int screenWidth, int screenHeight, int bitmapWidth, int bitmapHeight)
{
	HRESULT hr = S_OK;
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	m_bitmapWidth = bitmapWidth;
	m_bitmapHeight = bitmapHeight;
	m_previousPosX = -1;
	m_previousPosY = -1;
	m_previousWidth = bitmapWidth;
	m_previousHeight = bitmapHeight;

	hr = InitializeBuffers(device);
	if (FAILED(hr))
		return hr;

	scaling[0] = 1.0f;
	scaling[1] = 1.0f;
	scaling[2] = 1.0f;

	D3D11_BUFFER_DESC bd;
	hr = InitializeBuffers(device);
	if (FAILED(hr))
		return hr;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(Propertys);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	hr = device->CreateBuffer(&bd, NULL, &m_BlockProperty);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"ErrorCreate Translation Buffer", L"Error_BlockClass", MB_OK);
		return false;
	}
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(Factors);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	hr = device->CreateBuffer(&bd, NULL, &m_RenderFactors);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"ErrorCreate RenderFactors Buffer", L"Error_BlockClass", MB_OK);
		return false;
	}
	rotation[0] = 0.0f;
	rotation[1] = 0.0f;
	rotation[2] = 0.0f;
	D3DXMatrixIdentity(&propertys.RotationMatrix);
	return true;
}

HRESULT Rectangle2DClass::InitializeBuffers(ID3D11Device* device)
{
	HRESULT hr;
	VertexClass* vertices;
	ULONG* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	int i;
	m_vertexCount = 6;
	m_indexCount = m_vertexCount;
	vertices = new VertexClass[m_vertexCount];
	if (vertices == nullptr)
		return E_FAIL;
	memset(vertices, 0, sizeof(VertexClass)*m_vertexCount);
	indices = new ULONG[m_indexCount];
	if (indices == nullptr)
		return E_FAIL;
	for (i = 0; i < m_indexCount; i++)
		indices[i] = i;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexClass)*m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	vertexData.pSysMem = vertices;
	hr = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_BlockBuffer);

	if (FAILED(hr))
		return hr;

	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(ULONG)*m_indexCount;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	hr = device->CreateBuffer(&indexBufferDesc,&indexData,&m_indexBuffer);
	if (FAILED(hr))
		return hr;

	SAFE_DELETE(vertices);
	SAFE_DELETE(indices);
}

HRESULT Rectangle2DClass::UpdateBuffers(ID3D11DeviceContext* context,int positionX,int positionY,int wid,int hei)
{
	HRESULT hr;
	float left, right, top, bottom;
	VertexClass* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexClass* verticesPtr;
	//Y axis is different from that of directX11 while doing 2D rendering .
	positionY = -positionY;
	if (wid == -1 && hei == -1)
	{
		wid = m_previousWidth;
		hei = m_previousHeight;
	}
	if ((positionX == m_previousPosX) && (positionY == m_previousPosY)&&(wid == m_previousWidth)&&(hei == m_previousHeight))
		return S_OK;
	m_previousPosX = positionX;
	m_previousPosY = positionY;
	m_previousWidth = wid;
	m_previousHeight = hei;
	m_bitmapWidth = wid;
	m_bitmapHeight = hei;
	left = (float)(m_screenWidth / -2.0f) + (float)positionX;
	right = left + (float)m_bitmapWidth;
	top = (float)(m_screenHeight / 2.0f) + (float)positionY;
	bottom = top - (float)m_bitmapHeight;
	left = 2.0 * left / m_screenWidth;
	right = 2.0 * right / m_screenWidth;
	top = 2.0 * top / m_screenHeight;
	bottom = 2.0 * bottom / m_screenHeight;
	vertices = new VertexClass[m_vertexCount];
	if (vertices == NULL) return E_FAIL;
	ZeroMemory(vertices, sizeof(vertices));
	vertices[0].pts._XYZ[0] = left;
	vertices[0].pts._XYZ[1] = top;
	vertices[0].pts._XYZ[2] = 0.0f;
	vertices[0].texture[0] = 0.0f;
	vertices[0].texture[1] = 0.0f;

	vertices[1].pts._XYZ[0] = right;
	vertices[1].pts._XYZ[1] = bottom;
	vertices[1].pts._XYZ[2] = 0.0f;
	vertices[1].texture[0] = 1.0f;
	vertices[1].texture[1] = 1.0f;

	vertices[2].pts._XYZ[0] = left;
	vertices[2].pts._XYZ[1] = bottom;
	vertices[2].pts._XYZ[2] = 0.0f;
	vertices[2].texture[0] = 0.0f;
	vertices[2].texture[1] = 1.0f;
//第二个三角
	vertices[3].pts._XYZ[0] = left;
	vertices[3].pts._XYZ[1] = top;
	vertices[3].pts._XYZ[2] = 0.0f;
	vertices[3].texture[0] = 0.0f;
	vertices[3].texture[1] = 0.0f;

	vertices[4].pts._XYZ[0] = right;
	vertices[4].pts._XYZ[1] = top;
	vertices[4].pts._XYZ[2] = 0.0f;
	vertices[4].texture[0] = 1.0f;
	vertices[4].texture[1] = 0.0f;

	vertices[5].pts._XYZ[0] = right;
	vertices[5].pts._XYZ[1] = bottom;
	vertices[5].pts._XYZ[2] = 0.0f;
	vertices[5].texture[0] = 1.0f;
	vertices[5].texture[1] = 1.0f;
	hr = context->Map(m_BlockBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr)) 
		return hr;
	verticesPtr = (VertexClass*) mappedResource.pData;
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexClass)*m_vertexCount));
	context->Unmap(m_BlockBuffer, 0);
	SAFE_DELETE(vertices); 
	return hr;
}
void Rectangle2DClass::SetTransparency(ID3D11Device *device, ID3D11DeviceContext * context, float trans)
{
	factors.transparent = trans;
	D3D11_MAPPED_SUBRESOURCE myresource;
	//设置透明度等等渲染参数
	context->Map(m_RenderFactors, 0, D3D11_MAP_WRITE_DISCARD, 0, &myresource);
	Factors *dptr2 = (Factors *)myresource.pData;
	*dptr2 = factors;
	context->Unmap(m_RenderFactors, 0);
	context->PSSetConstantBuffers(0, 1, &m_RenderFactors);
	return ;
}
void Rectangle2DClass::Render(ID3D11Device *device, ID3D11DeviceContext * context,int positionX,int positionY)
{
	HRESULT hr;
	UINT stride = sizeof(VertexClass);
	UINT offset = 0;

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

	SetTransparency(device, context, factors.transparent);
	hr = UpdateBuffers(context, positionX, positionY);
	if (FAILED(hr))
		return ;
	context->IASetVertexBuffers(0, 1, &m_BlockBuffer, &stride, &offset);
	context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->DrawIndexed(m_indexCount, 0, 0);
}

void Rectangle2DClass::SetPosition(float _x, float _y, float _z, ID3D11Device *device, ID3D11DeviceContext * context)
{
	HRESULT hr;
	pos[0] = _x;
	pos[1] = _y;
	pos[2] = _z;
	D3DXMatrixTranslation(&propertys.TranslationMatrix, pos[0], pos[1], pos[2]);
	Propertys *dptr;
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
	return;
}


void Rectangle2DClass::SetScaling(float _x, float _y, float _z, ID3D11Device *device, ID3D11DeviceContext * context)
{
	HRESULT hr;
	scaling[0] = _x;
	scaling[1] = _y;
	scaling[2] = _z;
	D3DXMatrixScaling(&propertys.ScalingMatrix, scaling[0], scaling[1], scaling[2]);
	Propertys *dptr;
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
	return;
}

void Rectangle2DClass::SetRotation(float yaw, float pitch, float roll, ID3D11Device* device, ID3D11DeviceContext* context)
{
	HRESULT hr;
	rotation[0] = yaw;
	rotation[1] = pitch;
	rotation[2] = roll;
	D3DXMatrixRotationYawPitchRoll(&propertys.RotationMatrix,
		rotation[0], rotation[1], rotation[2]);

	Propertys *dptr;
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
	return;
}