#pragma once
#include<D3D11.h>
#include<D3DX10math.h>
#include<string>
#include"BlockClass.h"
using namespace std;

void DrawLine(BlockClass*, ID3D11Device*, ID3D11DeviceContext*, 
	float var, float var0, float var1, float var2, float var3, float var4);

class BoneClass
{
public:
	BoneClass();
	~BoneClass();

	void SetName(string);
	bool isLeaf();
	bool isRoot();				//是否是启始节点
	bool SetParent(BoneClass*);
	void SetYawPitchRoll(float,float,float);
	void SetPosition(float,float,float);
	bool BindWithAsParent(BoneClass*);
	int GetSonNum();

	D3DXVECTOR3 GetPosition();
private:
	D3DXVECTOR3 position;//位置x,y,z
	D3DXVECTOR3 rotation;//旋转角度yaw,pitch,roll
	D3DXMATRIX tramat;	//平移矩阵
	D3DXMATRIX rotmat;	//旋转矩阵
	D3DXMATRIX ptramat;	//作为母节点的平移矩阵
	D3DXMATRIX protmat;	//作为母节点的旋转矩阵
	string name;		//骨骼节点的名字
	BoneClass* parent;	//父节点
	BoneClass* sons[10];
	int SonNum;
};

