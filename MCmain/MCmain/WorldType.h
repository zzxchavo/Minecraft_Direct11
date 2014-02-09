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
	bool canBeCreated;	//��ͼ�����Ƿ������ɣ�����Ϊtrue
	bool isWorldTypeVersioned;//���ڵ�ͼ�����Ƿ��а汾�š�
	int GeneratorVersion;//��ͼ�������汾
	int worldTypeID;	//�������͵�ID
	string worldType;	//��������

	WorldType& setCanBeCreated(bool);
	WorldType& setVersioned();
};
