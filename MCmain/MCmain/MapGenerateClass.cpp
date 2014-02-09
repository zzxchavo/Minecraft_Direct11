#include "MapGenerateClass.h"
#include <functional>
#include <string>
using namespace std;

MapGenerateClass::MapGenerateClass(void)
{
	MAX_HASH = 5000;
	Generator = rand();
	cols = NULL;
	cols = new column[MAX_HASH];
	seed = 0xfcad12bdfcad12bd;
}


MapGenerateClass::~MapGenerateClass(void)
{
	if(cols)
	{
		delete [] cols;
		cols = NULL;
	}
}


MapGenerateClass::column &MapGenerateClass::operator[](const int x)
{
	return cols[x];
}

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

bool MapGenerateClass::prandom(float _p)//按照概率进行随机
{
	_p*=1000.0f;
	if(rand()%1000 < ((int)_p))
		return true;
	return false;
}

struct bfs_use
{
	int x,y;
};

void MapGenerateClass::GenerateArea(int x,int y,int chunks)
{
	for(int i=0;i<100;i++)
	for (int j = 0; j < 100; j++)
	{
		cols[BKDHash(i-50,j-50)].SetData(60,STONE);
	}
	return;
}
