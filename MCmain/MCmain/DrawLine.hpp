#pragma once
#include<D3D11.h>
#include<D3DX11.h>
#include"VertexClass.h"
#include"ShaderManager.h"
#include"Defines.h"

class Line
{
public:
	Line()
	{
		Inited = false;
	}
	~Line()
	{
		;
	}
	bool GetInitState()
	{
		return Inited;
	}
	HRESULT Initialize(ID3D11Device* device,ID3D11DeviceContext* context,VertexClass& one,VertexClass& two)
	{
		HRESULT hr = S_OK;
		VertexClass* vertices;
		ULONG* indices;
		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
		int i;
		m_vertexCount = 2;
		m_indexCount = m_vertexCount;
		vertices = new VertexClass[m_vertexCount];
		if (vertices == nullptr)
			return E_FAIL;
		memset(vertices, 0, sizeof(VertexClass)*m_vertexCount);
		memcpy(vertices[0].pts._XYZ, one.pts._XYZ, 3 * sizeof(float));
		memcpy(vertices[1].pts._XYZ, two.pts._XYZ, 3 * sizeof(float));

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

		hr = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
		if (FAILED(hr))
			return hr;

		SAFE_DELETE(vertices);
		SAFE_DELETE(indices);
		Inited = true;
		return hr;
	}
	HRESULT UpdateBuffers(ID3D11DeviceContext* context, VertexClass& one,VertexClass& two)
	{
		HRESULT hr;
		float left, right, top, bottom;
		VertexClass* vertices;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		VertexClass* verticesPtr;
		//Y axis is different from that of directX11 while doing 2D rendering .
		vertices = new VertexClass[m_vertexCount];
		if (vertices == NULL) return E_FAIL;
		ZeroMemory(vertices, sizeof(vertices));
		vertices[0].pts._XYZ[0] = one.pts._XYZ[0];
		vertices[0].pts._XYZ[1] = one.pts._XYZ[1];
		vertices[0].pts._XYZ[2] = one.pts._XYZ[2];
		vertices[0].texture[0] = 0.0f;
		vertices[0].texture[1] = 0.0f;

		vertices[1].pts._XYZ[0] = two.pts._XYZ[0];
		vertices[1].pts._XYZ[1] = two.pts._XYZ[1];
		vertices[1].pts._XYZ[2] = two.pts._XYZ[2];
		vertices[1].texture[0] = 1.0f;
		vertices[1].texture[1] = 1.0f;
		hr = context->Map(m_BlockBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(hr))
			return hr;
		verticesPtr = (VertexClass*)mappedResource.pData;
		memcpy(verticesPtr, (void*)vertices, (sizeof(VertexClass)*m_vertexCount));
		context->Unmap(m_BlockBuffer, 0);
		SAFE_DELETE(vertices);
		return hr;
	}
	void Render(ID3D11Device *device, ID3D11DeviceContext * context)
	{
		HRESULT hr;
		UINT stride = sizeof(VertexClass);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &m_BlockBuffer, &stride, &offset);
		context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		context->DrawIndexed(m_indexCount, 0, 0);
	}
private:
	bool Inited;
	ID3D11Buffer* m_BlockProperty;
	ID3D11Buffer* m_RenderFactors;
	ID3D11Buffer* m_BlockBuffer;
	ID3D11Buffer* m_indexBuffer;
	int m_vertexCount, m_indexCount;
	int m_screenWidth, m_screenHeight;
	int m_bitmapWidth, m_bitmapHeight;
	int m_previousPosX, m_previousPosY;
	int m_previousWidth, m_previousHeight;
};

void DrawLine(ID3D11Device* device, ID3D11DeviceContext* context,
	float var, float var0, float var1, float var2, float var3, float var4);