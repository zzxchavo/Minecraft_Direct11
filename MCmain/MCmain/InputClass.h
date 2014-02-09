#pragma once
class InputClass
{
public:
	InputClass(void);
	~InputClass(void);

	bool GetKeyState(char);
	void SetKeyState(char,bool);
private:
	bool keys[256];
};

