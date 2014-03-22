#include "InputClass.h"

InputClass::InputClass(void)
{
	m_lastMouseX = 0;
	m_lastMouseY = 0;
	mouseX = 0;
	mouseY = 0;
	memset(keys,0,sizeof(keys));
	memset(mouse,0,sizeof(mouse));
}


InputClass::~InputClass(void)
{
}

bool InputClass::GetKeyState(char x)
{
	return keys[x];
}

void InputClass::SetKeyState(char x,bool f)
{
	keys[x] = f;
}

float InputClass::GetMouseX()
{
	return mouseX;
}

float InputClass::GetMouseY()
{
	return mouseY;
}

void InputClass::SetMouseX(float mx)
{
	m_lastMouseX = mouseX;
	mouseX = mx;
	return;
}

void InputClass::SetMouseY(float my)
{
	m_lastMouseY = mouseY;
	mouseY = my;
	return;
}

void InputClass::SetMouseBtnState(int index,bool state)
{
	mouse[index] = state;
	return;
}

bool InputClass::GetMouseBtnState(int index)
{
	return mouse[index];
}

void InputClass::SetCentre(POINT& x)
{
	OutsideCentre = x;
}
POINT InputClass::GetCentre()
{
	return OutsideCentre;
}


float InputClass::GetLastMouseX()
{
	return m_lastMouseX;
}
float InputClass::GetLastMouseY()
{
	return m_lastMouseY;
}