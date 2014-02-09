#include "InputClass.h"
#include<memory.h>

InputClass::InputClass(void)
{
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
