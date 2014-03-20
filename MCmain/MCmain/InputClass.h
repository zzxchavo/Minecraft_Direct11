#pragma once
class InputClass
{
public:
	InputClass(void);
	~InputClass(void);

	bool GetKeyState(char);
	void SetKeyState(char,bool);
	
	float GetMouseX();
	float GetMouseY();
	
	void SetMouseX(float);
	void SetMouseY(float);
private:
	float mouseX,mouseY;
	bool keys[256];
};

