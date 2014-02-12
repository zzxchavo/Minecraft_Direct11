#pragma once
#include<queue>
#include<future>
#include<memory>
#include"DefinesAndTextures.h"

#define MAX 8*8*8

size_t myHash(int x,int y,int z);
int GetBlockPos(int x);
class TerrainBlock
{
public:
	TerrainBlock();
	TerrainBlock(int newx,int newy,int newz);
	~TerrainBlock();
	int GetX();
	int GetY();
	int GetZ();
	int& operator[](const int index);
	int GetBlock(int x,int y,int z);
	const bool isReady();
	const bool isEmpty();
	void SetPlace(int newx,int newy,int newz);
private:
	size_t privateHash(int x,int y,int z);
	int datas[MAX];
	int _x,_y,_z;
	bool _empty;
	bool _ready;
	bool UpdateOperation(int newx, int newy,int newz);
};

