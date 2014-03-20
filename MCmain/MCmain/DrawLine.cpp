#include"DrawLine.hpp"

Line m_line;

void DrawLine(ID3D11Device* device, ID3D11DeviceContext* context,
	float var, float var0, float var1, float var2, float var3, float var4)
{
	VertexClass one, two;
	one.pts._XYZ[0] = var;
	one.pts._XYZ[1] = var0;
	one.pts._XYZ[2] = var1;
	two.pts._XYZ[0] = var2;
	two.pts._XYZ[1] = var3;
	two.pts._XYZ[2] = var4;
	if (m_line.GetInitState() == false)
		m_line.Initialize(device, context, one, two);
	m_line.UpdateBuffers(context, one, two);
	m_line.Render(device, context);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


