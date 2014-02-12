#pragma 
#include<time.h>
#include<stdlib.h>
#include"DefinesAndTextures.h"
#define MAX_TIME 240

class DayTime
{
public:
	DayTime();
	~DayTime();

	HRESULT Initialize(ID3D11Device* device,ID3D11DeviceContext* context);

	void UpdateTime(ID3D11Device* device, ID3D11DeviceContext* context);
	long GetTime();
	int  GetSkyboxTexture();
private:
	long lasttime;
	long nowtime;
	long _time;
	int _skytex;
	ID3D11Buffer* m_skycolor;
	struct skycolor
	{
		float r, g, b, a;
	};
};
