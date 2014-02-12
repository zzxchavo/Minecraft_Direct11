#include "TerrainBlock.h"

size_t myHash(int x, int y, int z)
{
//	size_t hashcode = x ^ (y << 1) ^ (z << 1);
	size_t hashcode = 25 * x + 5 * y + z;
	return hashcode;
}
int GetBlockPos(int x)
{
//	if (tx < 0)
	int tx = x / 8;
	if(x < 0)
		if(x % 8 != 0)
			tx --;
/*	if (tx >= 0)
	if (tx * 8 - x < 0)
		tx++;
*/	return tx;
}
TerrainBlock::TerrainBlock()
{
	_x = 0;
	_y = 0;
	_empty = true;
	memset(datas, 0, sizeof(datas));
	UpdateOperation(0, 0, 0);
	_ready = true;
}


TerrainBlock::TerrainBlock(int newx,int newy,int newz)
{
	_x = newx;
	_y = newy;
	_z = newz;
	_empty = true;
	memset(datas,0,sizeof(datas));
	UpdateOperation(newx, newy,newz);
	_ready = true;
}


TerrainBlock::~TerrainBlock()
{

}

int& TerrainBlock::operator[](const int index)
{
	return datas[index];
}

int TerrainBlock::GetBlock(int x, int y, int z)
{
//	printf("%d %d %d\n", _x * 8 + x, _y*8+y, _z*8+z);
	if (x < 0 || x>8) return 0;
	if (y < 0 || y>8) return 0;
	if (z < 0 || z>8) return 0;
	return datas[privateHash(x, y, z)];
}
const bool TerrainBlock::isReady()
{
	return _ready;
}


size_t TerrainBlock::privateHash(int x, int y, int z)
{
	size_t hashcode = x * 64 + y * 8 + z;
	return hashcode;
}

const bool TerrainBlock::isEmpty()
{
	return _empty;
}

bool TerrainBlock::UpdateOperation(int newx,int newy,int newz)
{
	_x = newx;
	_y = newy;
	_z = newz;
	_empty = true;
	memset(datas,0,sizeof(datas));
	for (int i = 0; i < 8; i++)
	for (int j = 0; j < 8; j++)
	{
		size_t phash = privateHash(i, (_x+8)%8 , j);
		datas[phash] = []()->int{
			return rand() % 3 + 1;
	}();
	if (_empty && phash != TEX_EMPTY)
		{
			_empty = false;
		}
	}
	return true;
}

void TerrainBlock::SetPlace(int newx,int newy,int newz)
{
	UpdateOperation(newx,newy,newz);
	return ;
}

int TerrainBlock::GetX()
{
	return _x;
}
int TerrainBlock::GetY()
{
	return _y;
}
int TerrainBlock::GetZ()
{
	return _z;
}