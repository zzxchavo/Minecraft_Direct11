#include "DayTime.h"

float daycol[] = { 140, 186, 250 };
float dawncol[] = { 170, 128, 63 };
float nightcol[] = { 18, 24, 32 };


DayTime::DayTime()
{
	lasttime = clock();
	nowtime = lasttime;
	_time = rand() % MAX_TIME;
	_skytex = SKYBOX_DAY;
}


DayTime::~DayTime()
{
}
HRESULT DayTime::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
	HRESULT hr;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(skycolor);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	hr = device->CreateBuffer(&bd,NULL,&m_skycolor);
	if (FAILED(hr)) return hr;
	skycolor* dptr;
	skycolor data = {1.0f,1.0f,1.0f,1.0f};
	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(m_skycolor,0,D3D11_MAP_WRITE_DISCARD,0,&resource);
	dptr = (skycolor*)resource.pData;
	*dptr = data;
	context->Unmap(m_skycolor, 0);
	context->PSSetConstantBuffers(1, 1, &m_skycolor);
	return S_OK;
}


void DayTime::UpdateTime(ID3D11Device* device, ID3D11DeviceContext* context)
{
	nowtime = clock();
	long dt = nowtime - lasttime;
	if (dt >= 1000)
	{
		_time += (dt / 1000);
		lasttime = nowtime;
	}
	_time %= MAX_TIME;
	float period = (float)_time / MAX_TIME;
	float daypart = 0.0f;
	float dawnpart = 0.0f;
	float nightpart = 0.0f;
	if (period > 0.0f && period <= 0.23)
	{
		nightpart = 1.0f;
		_skytex = SKYBOX_NIGHT;
	}
	else
	{
		if (period > 0.23f && period <= 0.53f)
		{
			daypart = 1.0f;
			_skytex = SKYBOX_DAY;
		}
		else
		if (period > 0.53 && period <= 1.0f)
		{
			nightpart = 1.0f;
			_skytex = SKYBOX_NIGHT;
		}
	}
	float finalcolor[3] = {daypart * daycol[0] + dawnpart * dawncol[0] + nightpart * nightcol[0],
		daypart * daycol[1] + dawnpart * dawncol[1] + nightpart * nightcol[1] ,
		daypart * daycol[2] + dawnpart * dawncol[2] + nightpart * nightcol[2] };
	skycolor* dptr;
	skycolor data = { finalcolor[0], finalcolor[1], finalcolor[2] , 1.0f };
	while (data.r > 255) data.r -= 255;
	while (data.g > 255) data.g -= 255;
	while (data.b > 255) data.b -= 255;
	data.r /= 255.0f;
	data.g /= 255.0f;
	data.b /= 255.0f;
	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(m_skycolor, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	dptr = (skycolor*)resource.pData;
	*dptr = data;
	context->Unmap(m_skycolor, 0);
	context->PSSetConstantBuffers(1, 1, &m_skycolor);
	return;
}

long DayTime::GetTime()
{
	return _time;
}

int DayTime::GetSkyboxTexture()
{
	return _skytex;
}