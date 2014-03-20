#include "CloudClass.h"

SingleClass::SingleClass()
{
	error = -1;
}

SingleClass::~SingleClass()
{
	return ;
}

bool prandom(float _p)//按照概率进行随机
{
	_p*=1000.0f;
	if(rand()%1000 < ((int)_p))
		return true;
	return false;
}

HRESULT SingleClass::Initialize(int x,int z)
{
	memset(vis,0,sizeof(vis));
	pos2D startPosition;
	HRESULT hr = S_OK;
	pos2D start = {4,4};
	que.push(start);
	int moves[][2] = {{0,1},{0,-1},{1,0},{-1,0}};
	int cnt = 0;
	while(!que.empty()&&cnt<MAX_CLOUD_BLOCK&&prandom(0.95))
	{
		pos2D now = que.front();
		que.pop();
		for(int i=0;i<4;i++)
		{
			int newx = now.x+moves[i][0];
			int newz = now.z+moves[i][1];

			if((newx<MAX_CLOUD_WIDTH&&newx>=0&&newz<MAX_CLOUD_WIDTH&&newz>=0)&&!vis[newx][newz])
			{
				vis[newx][newz] = true;
				pos2D node = {newx,newz};
				que.push(node);
				node.x += x;
				node.z += z;
				Output.push_back(node);
				++cnt;
			}
		}
	}

	while(!que.empty()) que.pop();
	return hr;
}


int &SingleClass::GetQueX(const int index)
{
	return Output[index].x;
}

int &SingleClass::GetQueZ(const int index)
{
	return Output[index].z;
}

unsigned int SingleClass::GetQueSize()
{
	return Output.size();
}

float &SingleClass::GetX()
{
	return pos._XYZ[0];
}

float &SingleClass::GetY()
{
	return pos._XYZ[1];
}

float &SingleClass::GetZ()
{
	return pos._XYZ[2];
}


CloudClass::CloudClass(void)
{

}


CloudClass::~CloudClass(void)
{

}


void CloudClass::Render(ID3D11Device* device,ID3D11DeviceContext* context,BlockClass * m_block)
{
	m_block->SetScaling(10,0.001,10,device,context);
	m_block->SetRotation(0.0f, 0.0f, 0.0f, device, context);
	m_block->SetTransparency(device, context, 0.7);
	for(int i=0;i<MAX_CLOUD_NUM;i++)
	{
		int size = pos[i].GetQueSize();
		for(int j=0;j<size;j++)
		{
			m_block->SetPosition(10 * pos[i].GetQueX(j), 150, 10 * pos[i].GetQueZ(j), device, context);
			m_block->Render(device, context);
		}
	}
	m_block->SetScaling(1, 1, 1, device, context);
	m_block->SetRotation(0.0f, 0.0f, 0.0f, device, context);
}

HRESULT CloudClass::Initialize(ID3D11Device * device,ID3D11DeviceContext * context)
{
	HRESULT hr = S_OK;
	WindDir = rand()%4;
	for(int i=0;i<MAX_CLOUD_NUM;i++)
	{
		pos[i].Initialize((rand()%20)-10,(rand()%20)-10);
	}
	return hr;
}