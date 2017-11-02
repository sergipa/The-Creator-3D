#pragma once
#include "Module.h"


class ModuleTime :
	public Module
{
public:
	ModuleTime(Application* app, bool start_enabled = true, bool is_game = false);
	~ModuleTime();

	update_status PreUpdate(float dt);

	float GetGameDt() const;
	float GetScale() const;
	bool SetScale(float new_scale);
public:
	
private:
	float time_scale;
	float game_dt;
};

