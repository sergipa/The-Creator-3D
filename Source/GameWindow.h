#pragma once

#include "Window.h"

class GameWindow :
	public Window
{
public:
	GameWindow();
	virtual ~GameWindow();

	void DrawWindow();

public:
	float game_scene_width;
	float game_scene_height;
};

