#include "ModuleTime.h"



ModuleTime::ModuleTime(Application* app, bool start_enabled, bool is_game) : Module(app, start_enabled, is_game)
{
	name = "Time";

	time_scale = 1.0f;
	game_dt = 0.0f;
}


ModuleTime::~ModuleTime()
{
}

update_status ModuleTime::PreUpdate(float dt)
{
	game_dt = dt * time_scale;


	return UPDATE_CONTINUE;
}

float ModuleTime::GetGameDt() const
{
	return game_dt;
}

float ModuleTime::GetScale() const
{
	return time_scale;
}

bool ModuleTime::SetScale(float new_scale)
{
	if (new_scale >= 0.0f)
	{
		time_scale = new_scale;
		return true;
	}
	else
		return false;
}
