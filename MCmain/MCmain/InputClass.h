#pragma once
#include<Windows.h>
#include<memory.h>
#define MOUSE_LEFTBUTTON 0
#define MOUSE_RIGHTBUTTON 1
#define MOUSE_MIDDLEBUTTON 2

class InputClass
{
public:
	InputClass(void);
	~InputClass(void);

	bool GetKeyState(char);
	void SetKeyState(char,bool);
	
	float GetMouseX();
	float GetMouseY();
	float GetLastMouseX();
	float GetLastMouseY();
	
	void SetMouseX(float);
	void SetMouseY(float);

	void SetMouseBtnState(int index,bool state);
	bool GetMouseBtnState(int index);

	void SetCentre(POINT& x);
	POINT GetCentre();
private:
	POINT OutsideCentre;
	float m_lastMouseX, m_lastMouseY;
	float mouseX,mouseY;
	bool mouse[3];
	bool keys[256];
};

