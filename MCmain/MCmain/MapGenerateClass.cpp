#include "MapGenerateClass.h"
#include <functional>
#include <string>
using namespace std;

MapGenerateClass::MapGenerateClass(void)
{
	MAX_HASH = 5000;
	Generator = rand();
	seed = 0xfcad12bdfcad12bd;
	_x = 0;_y = 0;_z = 0;
	for (int i = -2; i < 3; i++)
	for (int j = -2; j < 3; j++)
	for (int k = -2; k < 3; k++)
		datas[myHash(i + 2,j + 2,k + 2)].SetPlace(_x + i,_y + j,_z + k);
}


MapGenerateClass::~MapGenerateClass(void)
{
}


TerrainBlock& MapGenerateClass::operator[](const int index)
{
	return datas[index];
}
/*
size_t MapGenerateClass::BKDHash(column *pt)
{
	string HashStr = std::to_string(pt->data[0]);
	HashStr += std::to_string(pt->data[1]);
	register size_t Hash = 0;
	std::hash<string>hasher;
	Hash = hasher(HashStr);
	return Hash%MAX_HASH;
}

size_t MapGenerateClass::BKDHash(int x,int y)
{
	size_t Hash = 0;
	string HashStr = std::to_string(x);
	HashStr += std::to_string(y);
	std::hash<string>hasher;
	Hash = hasher(HashStr);
	return Hash%MAX_HASH;
}

size_t MapGenerateClass::BKDHash(int x,int y,int z)
{
	size_t Hash = 0;
	string HashStr = std::to_string(x);
	HashStr += std::to_string(y);
	HashStr += std::to_string(z);
	std::hash<string>hasher;
	Hash = hasher(HashStr);
	return Hash%MAX_HASH;
}

void MapGenerateClass::GeneratePosition(int x,int z)
{
	size_t HashCode = BKDHash(x,z);
	if(cols[HashCode].empty == true)
		return ;
	for(int i=0;i<cols[HashCode].Max_height;i++)
	{
		if(i<55&&i>=45)
		{
			if(prandom(0.7))
				cols[HashCode].SetData(i,SAND);
			else
			{
				if(prandom(0.3))
					cols[HashCode].SetData(i,CLAY);
				else
					cols[HashCode].SetData(i,SAND);
			}
		}
		else
		{
			if(i<50)
			{
				if(prandom(0.3))
					cols[HashCode].SetData(i,0);
				else
					cols[HashCode].SetData(i,STONE);
			}
			else
			{
				if(prandom(0.3))
					cols[HashCode].SetData(i,GRASS_TOP);
				else
					cols[HashCode].SetData(i,STONE);
			}
		}
	}
	cols[HashCode].empty = true;
	return ;
}

void  MapGenerateClass::GeneratePosition(size_t HashCode)
{
	if(cols[HashCode].empty == true)
		return ;
	for(int i=0;i<cols[HashCode].Max_height;i++)
	{
		if(i<55&&i>=45)
		{
			if(prandom(0.7))
				cols[HashCode].SetData(i,SAND);
			else
			{
				if(prandom(0.3))
					cols[HashCode].SetData(i,CLAY);
				else
					cols[HashCode].SetData(i,SAND);
			}
		}
		else
		{
			if(i<50)
			{
				if(prandom(0.3))
					cols[HashCode].SetData(i,0);
				else
					cols[HashCode].SetData(i,STONE);
			}
			else
			{
				if(prandom(0.3))
					cols[HashCode].SetData(i,GRASS_TOP);
				else
					cols[HashCode].SetData(i,STONE);
			}
		}
	}
	cols[HashCode].empty = true;
}

void MapGenerateClass::GenerateArea(int x,int y,int chunks)
{
	for(int i=0;i<100;i++)
	for (int j = 0; j < 100; j++)
	{
		cols[BKDHash(i-50,j-50)].SetData(60,STONE);
	}
	return;
}

*/
bool MapGenerateClass::prandom(float _p)//按照概率进行随机
{
	_p*=1000.0f;
	if(rand()%1000 < ((int)_p))
		return true;
	return false;
}

int MapGenerateClass::GetData(int x,int y,int z)
{
	int tx,ty,tz;
	tx = GetBlockPos(x);
	ty = GetBlockPos(y);
	tz = GetBlockPos(z);
//	printf("%d %d %d\n", tx - _x,ty- y,tz-z);
	if (tx + 2 - _x < 0 || tx + 2 - _x > 4) return 0;
	if (ty + 2 - _y < 0 || ty + 2 - _y > 4) return 0;
	if (tz + 2 - _z < 0 || tz + 2 - _z > 4) return 0;
	size_t firstHash = myHash(tx - _x + 2 ,ty - _y + 2,tz - _z + 2);
	tx = datas[firstHash].GetX() * 8;
	ty = datas[firstHash].GetY() * 8;
	tz = datas[firstHash].GetZ() * 8;
//	if (x - tx < 0) printf("x");
//	if (y - ty < 0) printf("y");
//	if (z - tz < 0) printf("z");
	return datas[firstHash].GetBlock(((x%8)+8)%8,
		((y % 8) + 8) % 8,
		((z % 8) + 8) % 8);
}