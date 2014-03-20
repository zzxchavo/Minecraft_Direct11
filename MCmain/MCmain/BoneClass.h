#pragma once
#include<D3D11.h>
#include<D3DX10math.h>
#include<string>
#include"BlockClass.h"
using namespace std;


class BoneClass
{
public:
	BoneClass();
	~BoneClass();

	void SetName(string);
	bool isLeaf();
	bool isRoot();				//�Ƿ�����ʼ�ڵ�
	bool SetParent(BoneClass*);
	void SetYawPitchRoll(float,float,float);
	void SetPosition(float,float,float);
	bool BindWithAsParent(BoneClass*);
	int GetSonNum();

	D3DXVECTOR3 GetPosition();
private:
	D3DXVECTOR3 position;//λ��x,y,z
	D3DXVECTOR3 rotation;//��ת�Ƕ�yaw,pitch,roll
	D3DXMATRIX tramat;	//ƽ�ƾ���
	D3DXMATRIX rotmat;	//��ת����
	D3DXMATRIX ptramat;	//��Ϊĸ�ڵ��ƽ�ƾ���
	D3DXMATRIX protmat;	//��Ϊĸ�ڵ����ת����
	string name;		//�����ڵ������
	BoneClass* parent;	//���ڵ�
	BoneClass* sons[10];
	int SonNum;
};

