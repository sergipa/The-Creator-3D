#pragma once

#include "p2List.h"
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModuleImGui.h"
#include "MathGeoLib\Algorithm\Random\LCG.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleScene* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModulePhysics3D* physics;
	ModuleImGui* imgui;
private:

	Timer	ms_timer;
	float	dt;
	p2List<Module*> list_modules;
	LCG*	random = nullptr;
	
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	LCG& RandomNumber();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};