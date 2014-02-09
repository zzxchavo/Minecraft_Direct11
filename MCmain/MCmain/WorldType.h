#pragma once
#include"World.h"
#include<string>
using namespace std;

bool equalsIgnoreCase(char* a, char* b);
bool equalsIgnoreCase(char* a, string b);
bool equalsIgnoreCase(string a, char* b);
bool equalsIgnoreCase(string a, string b);

class WorldType
{
public:
	WorldType();
	WorldType(int, char*);
	WorldType(int, char*,int);
	~WorldType();
	static WorldType* worldTypes;
	static WorldType* DEFAULT;
	static WorldType* FLAT;
	static WorldType* LARGE_BIOMES;
	static WorldType* DEFAULT_1_1;

	string getWorldTypeName();
	string getTranslateName();
	int getGeneratorVersion();
	int getWorldTypeID();
	bool getCanBeCreated();
	bool isVersioned();
	WorldType& getWorldTypeForGeneratorVersion(int par1);
	WorldType& parseWorldType(char*);
	
	int getMinimumSpawnHeight(World &world);
	double getHorizon(World&);
//	WorldChunkMannager getChunkManager();

private:
	bool canBeCreated;	//地图类型是否能生成，正常为true
	bool isWorldTypeVersioned;//现在地图类型是否有版本号。
	int GeneratorVersion;//地图生成器版本
	int worldTypeID;	//世界类型的ID
	string worldType;	//世界类型

	WorldType& setCanBeCreated(bool);
	WorldType& setVersioned();
};
