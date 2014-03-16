#pragma once
#include<queue>
#include<memory.h>
#include<cstdlib>
#include<Windows.h>
#include<stdio.h>
#include"PointClass.h"
#include"TerrainBlock.h"
#include"MapGenerateClass.h"
#include"Defines.h"

using namespace std;


class MapGenerateClass
{
public:
	MapGenerateClass(void);
	~MapGenerateClass(void);
	enum HASH_XY_TYPE
	{
		X,Y
	};
	struct column
	{
		column()
		{
			empty = false;
			Max_height = 0;
			memset(data,0,sizeof(data));
		}
		char GetData(int lev)
		{
			if(lev < 0) return 0;
			unsigned int * pPosition = (data + 2);
			int d1 = lev / 8 , d2 = (lev % 8);
			return ((pPosition[d1]>>(4*(7-d2)))&(0xF));
		}

		int SetData(int lev,char c)
		{
			if(lev < 0) return 0;
			int i;
			unsigned int * pPosition = (data + 2);
			int d1 = lev / 8 , d2 = (lev % 8);
			if (d1 > 37) 
				d1 = 37;
			unsigned int pre,suf,tdata = pPosition[d1];
			c%=0x100;
			pre = 4*d2;
			suf = 4*(7-d2);
			unsigned int suffix = tdata<<(pre+4);
			suffix>>=(pre+4);
			unsigned int prefix = tdata>>(suf+4);
			if(pre == 0)
			{
				suffix = tdata<<4;
				suffix >>= 4;
				prefix = 0;
			}
			if(suf == 0)
			{
				suffix = 0;
				prefix = tdata>>4;
			}
			prefix<<=4;
			prefix+=c;
			prefix<<=suf;
			pPosition[d1] = prefix+suffix;
			return 0;
		}
		int GetData(HASH_XY_TYPE ch)
		{
			if(ch == X)
				return (int)data[0];
			if(ch == Y)
				return (int)data[1];
		}
		void operator &=(const column & other)
		{
			memcpy(this,&other,sizeof(column));
		}
		unsigned int data[40];
		unsigned int Max_height;
		bool empty;
	};
	bool prandom(float);//按照概率进行随机
	size_t GetSeed();
//	size_t BKDHash(column *);
//	size_t BKDHash(int,int);
//	size_t BKDHash(int,int,int);
//	void GeneratePosition(int,int);//务必先给column::Max_height初始化
//	void GeneratePosition(unsigned int);
//	void GenerateArea(int,int,int);//给一片区域进行初始化
	TerrainBlock& operator [](const int);
	int GetData(int x,int y,int z);
private:
	int _x, _y, _z;	//人物所在位置/8的中心大块
	size_t seed;
	size_t Generator;
	unsigned int MAX_HASH;
	TerrainBlock datas[5*5*5];
};

