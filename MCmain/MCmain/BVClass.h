#pragma once

#define BV_AABB		0	//AABB
#define BV_OBB		1	//OBB
#define BV_SPHERE	2	//Sphere
#define BV_SSV		3	//Sphere minkowski unin
#define BV_			4	//Sphere

class BVClass
{
public:
	BVClass(void);
	~BVClass(void);
	
	void Set();
	
	virtual bool Test(BVClass *) = 0;
private:
	unsigned int type;
};

