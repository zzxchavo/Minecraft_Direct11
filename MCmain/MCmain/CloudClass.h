#pragma once
#include"PointClass.h"
#include"BlockClass.h"
#include <stdlib.h>
#include <memory.h>
#include <queue>
#include <vector>
#include <Windows.h>

#define MAX_CLOUD_NUM 5
#define MAX_CLOUD_BLOCK 50
#define MAX_CLOUD_WIDTH 10

class SingleClass
{
public:
	SingleClass();
	~SingleClass();
	HRESULT Initialize(int,int);
	
	int &GetQueX(const int);
	int &GetQueZ(const int);

	unsigned int GetQueSize();

	float &GetX();
	float &GetY();
	float &GetZ();
private:	
	int error;
	bool vis[MAX_CLOUD_WIDTH][MAX_CLOUD_WIDTH];
	struct pos2D
	{
		int x;
		int z;
	};
	std::queue<pos2D>que;
	std::vector<pos2D>Output;
	PointClass pos;
};

class CloudClass
{
public:
	CloudClass(void);
	~CloudClass(void);
	void Draw(ID3D11Device * device,ID3D11DeviceContext*,BlockClass*);
	HRESULT Initialize(ID3D11Device * device,ID3D11DeviceContext*);
private:
	unsigned int WindDir;
	SingleClass pos[MAX_CLOUD_NUM];
};

