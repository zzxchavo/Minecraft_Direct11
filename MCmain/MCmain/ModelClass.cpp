#include "ModelClass.h"
bool isNum(char x)
{
	return x>='0' && x<='9';
}
bool CatchFace(std::string& str, unsigned int * vi, unsigned int * ui, unsigned int * vni)
{
	if (str[0] != 'f' || str[1] != ' ')
		return false;
	int cur;
	unsigned int ret = 0;
	int state = 1, i;
	int m = str.size(), a[3], c = 0;
	vi[0] = -1;
	ui[0] = -1;
	vni[0] = -1;
	for (i = 2; i<m&&str[i] != ' '; i++)
	{
		if (str[i] == '/')
		{
			state += 2;
			a[c++] = i;
		}
	}
	if (state == 5 && a[1] - a[0] != 1)
		state += 2;
	cur = 0;
	for (i = 2; i<m&&str[i] != ' '; i++)
	{
		if (str[i] == '\n' || str[i] == '\0')
			return true;
		if (state % 2)
		{
			for (ret = 0; i<m; i++)
			{
				if (str[i] == '/' || str[i] == ' ')
					break;
				if (isNum(str[i]))
				{
					ret *= 10;
					ret += str[i] - 48;
				}
			}
			vi[cur] = ret - 1;
		}
		else
			vi[cur] = -1;
		if ((state / 2) % 2)
		{
			for (ret = 0, i += 1; i<m; i++)
			{
				if (str[i] == '/' || str[i] == ' ' || str[i] == '\n')
					break;
				if (isNum(str[i]))
				{
					ret *= 10;
					ret += str[i] - 48;
				}
			}
			ui[cur] = ret - 1;
		}
		else
		{
			ui[cur] = -1;
			if (state / 4 % 2) i += 1;
		}
		if (state / 4 % 2)
		{
			for (ret = 0, i += 1; i<m; i++)
			{
				if (str[i] == '/' || str[i] == ' ' || str[i] == '\n')
					break;
				if (isNum(str[i]))
				{
					ret *= 10;
					ret += str[i] - 48;
				}
			}
			vni[cur] = ret - 1;
		}
		else
			vni[cur] = -1;
		++cur;
	}
	return true;
}

ModelClass::ModelClass()
{
}


ModelClass::~ModelClass()
{
}

HRESULT ModelClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* context,const char* filename)
{
	HRESULT hr;
	InitializeDatas(filename);

	SetTransparency(device, context, 1.0f);
	printf("[%d]\n", vec.size());
	for (int i = 0; i < vec.size(); i++)
		printf("[[%d]]\n", vec[i]->size());
	scaling[0] = 1.0f;
	scaling[1] = 1.0f;
	scaling[2] = 1.0f;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	m_indexBuffer = new ID3D11Buffer* [vec.size()];
	m_ModelBuffer = new ID3D11Buffer* [vec.size()];
	for (int i = 0;i < vec.size();i++)
	{
		if (vec[i]->size() == 0) continue;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VertexClass)* vec[i]->size();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA sb;
		ZeroMemory(&sb, sizeof(sb));
		sb.pSysMem = &(vec[i]->at(0));
		hr = device->CreateBuffer(&bd, &sb, &m_ModelBuffer[i]);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(Propertys);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	hr = device->CreateBuffer(&bd, NULL, &m_ModelProperty);
	if (FAILED(hr))
	{
		return hr;
	}
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(Factors);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	hr = device->CreateBuffer(&bd, NULL, &m_RenderFactors);
	if (FAILED(hr))
	{
		return hr;
	}
	rotation[0] = 0.0f;
	rotation[1] = 0.0f;
	rotation[2] = 0.0f;
	D3DXMatrixIdentity(&propertys.RotationMatrix);

	D3D11_BUFFER_DESC indexBufferDesc;
	indices = new ULONG [10000];
	for (int i = 0; i < 10000; i++)
		indices[i] = i;
	for (int i = 0; i < vec.size(); i++)
	{
		ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(ULONG)* vec[i]->size();
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		D3D11_SUBRESOURCE_DATA indexData;
		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;
		hr = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer[i]);
		if (FAILED(hr))
			return hr;
	}
	delete[] indices;
	Propertys *dptr;
	Factors    *dptr2;
	Propertys Combine;
	D3D11_MAPPED_SUBRESOURCE myresource;
	D3DXMatrixTranspose(&Combine.TranslationMatrix, &propertys.TranslationMatrix);
	D3DXMatrixTranspose(&Combine.ScalingMatrix, &propertys.ScalingMatrix);
	D3DXMatrixTranspose(&Combine.RotationMatrix, &propertys.RotationMatrix);
	hr = context->Map(m_ModelProperty, 0, D3D11_MAP_WRITE_DISCARD, 0, &myresource);
	dptr = (Propertys *)myresource.pData;
	*dptr = Combine;
	context->Unmap(m_ModelProperty, 0);
	context->VSSetConstantBuffers(2, 1, &m_ModelProperty);

	D3D11_MAPPED_SUBRESOURCE renderFac;
	//设置透明度等等渲染参数
	hr = context->Map(m_RenderFactors, 0, D3D11_MAP_WRITE_DISCARD, 0, &renderFac);
	dptr2 = (Factors *)renderFac.pData;
	*dptr2 = factors;
	context->Unmap(m_RenderFactors, 0);
	context->PSSetConstantBuffers(0, 1, &m_RenderFactors);
	return hr;
}

HRESULT ModelClass::InitializeDatas(const char* filename)
{
	HRESULT hr = S_OK;
	fp = fopen(filename,"r");
	if (fp == NULL)
	{
		MessageBox(NULL,L"",L"",MB_OK);
		return E_FAIL;
	}
	char buffer[1024];
	SAFE_CREATE(filetext, std::string);
	SAFE_CREATE(filetextcpy, std::string);
	while (fgets(buffer,1024,fp) != NULL)
	{
		int pos = 0;
		int max_length = strlen(buffer);
		bool isSharp = false;
		for (pos = 0; pos < max_length && buffer[pos] == ' '; pos++);
		if (buffer[pos] == '#') isSharp = true;
		if(isSharp == true)
			continue;
		*filetext += buffer;
	}
	cout << "step1 done!" << endl;
	*filetextcpy = *filetext;
	regex exp("(v\\s|vn|vt)\\s[\\d|\.|\-]+\\s[\\d|\.|\-]+\\s[\\d|\.|\-]+\\s");
	smatch res;
	FLOAT3 tem;
	while (regex_search(*filetext, res, exp, regex_constants::match_any))
	{
		string strgot = res[0];
		*filetext = res.suffix().str();
		sscanf(strgot.c_str(), "%*s%f%f%f", &tem.var[0], &tem.var[1], &tem.var[2]);
		if (strgot[0] == 'v')
		{
			if (strgot[1] == 'n')
			{
				nor.push_back(tem);
			}
			else if (strgot[1] == 't')
			{
				tex.push_back(tem);
			}
			else
			{
				pos.push_back(tem);
			}
		}
	}
	cout << "step2 done!" << endl;
	regex exp2("g\\s\\w+\\s");
	while (regex_search(*filetextcpy, res, exp2, regex_constants::match_any))
	{
		string strgot = res[0];
		*filetextcpy = res.suffix().str();
		strgot = strgot.substr(2);
		cout << strgot << endl;
		vector<VertexClass>* ptem;
		vec.push_back(ptem);
		SAFE_CREATE(vec.back(), vector<VertexClass>);
		name.push_back(strgot);
		mp[strgot] = vec.size() - 1; 
		//把顶点整理下顺序整合进去vec.back()
		int cnt = 0;
		for (int cur = 0;cur < (*filetextcpy).size();cur++)
		{
			if (filetextcpy->at(cur) == 'f') ++cnt;
			if (filetextcpy->at(cur) == 'g') break;
		}
		regex subexp("f\\s+\\d*\\D\\d*\\D\\d*\\s+\\d*\\D\\d*\\D\\d*\\s+\\d*\\D\\d*\\D\\d*\\s+");
		while (cnt-- && regex_search((*filetextcpy), res, subexp, regex_constants::match_any))
		{
			strgot = res[0];
			*filetextcpy = res.suffix().str();
			UINT vi[3], vni[3], vti[3];
			CatchFace(strgot,vi,vti,vni);
			for (int i = 0; i<3; i++)
			{
				VertexClass tver;
				if (vi[i] != -1)
				{
					tver.pts._XYZ[0] = pos[vi[i]].var[0];
					tver.pts._XYZ[1] = pos[vi[i]].var[1];
					tver.pts._XYZ[2] = pos[vi[i]].var[2];
				}
				else memset(tver.pts._XYZ, 0, sizeof(tver.pts._XYZ));
				if (vni[i] != -1)
				{
					tver.normal[0] = nor[vni[i]].var[0];
					tver.normal[1] = nor[vni[i]].var[1];
					tver.normal[2] = nor[vni[i]].var[2];
				}
				else memset(tver.normal,0,sizeof(tver.normal));
				if (vti[i] != -1)
				{
					tver.texture[0] = tex[vni[i]].var[0];
					tver.texture[1] = tex[vni[i]].var[1];
					tver.texture[2] = tex[vni[i]].var[2];
				}
				else memset(tver.texture,0,sizeof(tver.texture));
				printf("%d %d %d ",vi[0],vi[1],vi[2]);
//				printf("%f %f %f\n", tver.pts._XYZ[0], tver.pts._XYZ[1], tver.pts._XYZ[2]);
				vec.back()->push_back(tver);
			}
		}
	}
	pos.clear();
	nor.clear();
	tex.clear();
	SAFE_DELETE(filetext);
	SAFE_DELETE(filetextcpy);
	return hr;
}


HRESULT ModelClass::Shutdown()
{
	HRESULT hr = S_OK;
	SAFE_DELETE(filetext);
	for (auto & _param : vec)
	{
		SAFE_DELETE(_param);
	}
	vec.clear();
	return hr;
}

HRESULT ModelClass::Render(ID3D11Device* device, ID3D11DeviceContext* context)
{
	HRESULT hr = S_OK;
	UINT stride = sizeof(VertexClass);
	UINT offset = 0;

	Propertys *dptr;
	Factors    *dptr2;
	Propertys Combine;

	D3D11_MAPPED_SUBRESOURCE myresource;
	D3DXMatrixTranspose(&Combine.TranslationMatrix, &propertys.TranslationMatrix);
	D3DXMatrixTranspose(&Combine.ScalingMatrix, &propertys.ScalingMatrix);
	D3DXMatrixTranspose(&Combine.RotationMatrix, &propertys.RotationMatrix);
	hr = context->Map(m_ModelProperty, 0, D3D11_MAP_WRITE_DISCARD, 0, &myresource);
	dptr = (Propertys *)myresource.pData;
	*dptr = Combine;
	context->Unmap(m_ModelProperty, 0);
	context->VSSetConstantBuffers(2, 1, &m_ModelProperty);

	D3D11_MAPPED_SUBRESOURCE renderFac;
	//设置透明度等等渲染参数
	hr = context->Map(m_RenderFactors, 0, D3D11_MAP_WRITE_DISCARD, 0, &renderFac);
	dptr2 = (Factors *)renderFac.pData;
	*dptr2 = factors;
	context->Unmap(m_RenderFactors, 0);
	context->PSSetConstantBuffers(0, 1, &m_RenderFactors);
	for (int i = 0;i < vec.size();i++)
	{
		context->IASetVertexBuffers(0, 1, &m_ModelBuffer[i], &stride, &offset);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->IASetIndexBuffer(m_indexBuffer[i], DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(vec[i]->size(), 0, 0);
	}
	return hr;
}


void ModelClass::SetPosition(float _x, float _y, float _z, ID3D11Device*, ID3D11DeviceContext*)
{
	modelpos[0] = _x;
	modelpos[1] = _y;
	modelpos[2] = _z;
	D3DXMatrixTranslation(&propertys.TranslationMatrix, modelpos[0], modelpos[1], modelpos[2]);
	return;
}


void ModelClass::SetRotation(float yaw, float pitch, float roll, ID3D11Device*, ID3D11DeviceContext*)
{
	rotation[0] = yaw;
	rotation[1] = pitch;
	rotation[2] = roll;
	D3DXMatrixRotationYawPitchRoll(&propertys.RotationMatrix,
		rotation[0], rotation[1], rotation[2]);
}

void ModelClass::SetScaling(float _x, float _y, float _z, ID3D11Device*, ID3D11DeviceContext*)
{
	scaling[0] = _x;
	scaling[1] = _y;
	scaling[2] = _z;
	D3DXMatrixScaling(&propertys.ScalingMatrix, scaling[0], scaling[1], scaling[2]);
	return;
}

void ModelClass::SetTransparency(ID3D11Device *device, ID3D11DeviceContext * context, float trans)
{
	HRESULT hr;
	factors.transparent = trans;
	return;
}