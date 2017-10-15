#pragma once
#include "Window.h"
#include <string>

class InputConfigWindow :
	public Window
{
public:
	InputConfigWindow();
	~InputConfigWindow();

	void DrawWindow();
	int SearchInKeys(const char *keys[],std::string key);
private:
	std::string s_key_speed;
	std::string s_key_forward;
	std::string s_key_backward;
	std::string s_key_up;
	std::string s_key_down;
	std::string s_key_left;
	std::string s_key_right;
};

