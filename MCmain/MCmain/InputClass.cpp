#include "InputClass.h"
#include<memory.h>

InputClass::InputClass(void)
{
	mouseX = 0;
	mouseY = 0;
	memset(keys,0,sizeof(keys));
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
	mouseX = mx;
	return;
}

void InputClass::SetMouseY(float my)
{
	mouseY = my;
	return;
}