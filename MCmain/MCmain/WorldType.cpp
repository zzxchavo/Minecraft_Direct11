#include"WorldClass.h"
#include "WorldType.h"


WorldType* WorldType::worldTypes = new WorldType[16];
WorldType* WorldType::DEFAULT = new WorldType;
WorldType* WorldType::FLAT = new WorldType;
WorldType* WorldType::LARGE_BIOMES = new WorldType;
WorldType* WorldType::DEFAULT_1_1 = new WorldType;

WorldType::WorldType()
{
}

WorldType::WorldType(int par1, char* par2str)
{
	WorldType(par1,par2str,0);
}

WorldType::WorldType(int par1, char* par2str, int par3)
{
	this->worldType = par2str;
	this->GeneratorVersion = par3;
	this->canBeCreated = true;
	this->worldTypeID = par1;
	worldTypes[par1] = *this;
//biome设置，并没有做生物集群。
	switch (par1)
	{
		case 8:;
	}
}

WorldType::~WorldType()
{
}


string WorldType::getWorldTypeName()
{
	return (this->worldType);
}

string WorldType::getTranslateName()
{
	string ret = "generator" + this->worldType;
	return ret;
}

int WorldType::getGeneratorVersion()
{
	return GeneratorVersion;
}

WorldType& WorldType::getWorldTypeForGeneratorVersion(int par1)
{
	return *((this->DEFAULT && par1 == 0) ? DEFAULT_1_1 : this);
}

WorldType& WorldType::setCanBeCreated(bool par1)
{
	this->canBeCreated = par1;
	return *this;
}

bool WorldType::getCanBeCreated()
{
	return this->canBeCreated;
}

WorldType& WorldType::setVersioned()
{
	this->isWorldTypeVersioned= true;
	return *this;
}

bool WorldType::isVersioned()
{
	return isWorldTypeVersioned;
}

WorldType& WorldType::parseWorldType(char* par0str)
{
	int size = sizeof(this->worldTypes) / sizeof(WorldType);
	size = 16;
	for (int i = 0;i < size ; i++)
	{
		if (&worldTypes[i] != nullptr && equalsIgnoreCase(par0str, worldTypes[i].worldType))
		{
			return worldTypes[i];
		}
	}
	return worldTypes[0];
}

int WorldType::getWorldTypeID()
{
	return worldTypeID;
}

int WorldType::getMinimumSpawnHeight(World& world)
{
	return this->FLAT ? 4 : 64;
}

double WorldType::getHorizon(World& world)
{
	return this->FLAT ? 0.0 : 63.0;
}


bool equalsIgnoreCase(char* a, char* b)
{
	string sa = a;
	string sb = b;
	if (sa.size() != sb.size()) return false;
	for (const auto & _param : sa)
	{
		toupper(_param);
	}
	for (const auto& _param : sb)
	{
		toupper(_param);
	}
	return sa == sb;
}
bool equalsIgnoreCase(char* a, string b)
{
	string sa = a;
	string sb = b;
	if (sa.size() != sb.size()) return false;
	for (const auto & _param : sa)
	{
		toupper(_param);
	}
	for (const auto& _param : sb)
	{
		toupper(_param);
	}
	return sa == sb;
}
bool equalsIgnoreCase(string a, char* b)
{
	string sa = a;
	string sb = b;
	if (sa.size() != sb.size()) return false;
	for (const auto & _param : sa)
	{
		toupper(_param);
	}
	for (const auto& _param : sb)
	{
		toupper(_param);
	}
	return sa == sb;
}
bool equalsIgnoreCase(string a, string b)
{
	string sa = a;
	string sb = b;
	if (sa.size() != sb.size()) return false;
	for (const auto & _param : sa)
	{
		toupper(_param);
	}
	for (const auto& _param : sb)
	{
		toupper(_param);
	}
	return sa == sb;
}