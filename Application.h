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
//#include "ModuleImGui.h"
#include "MathGeoLib\Algorithm\Random\LCG.h"
#include "ModuleEditor.h"

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
	//ModuleImGui* imgui;
	ModuleEditor* editor;
private:

	Timer	ms_timer;
	Timer	fps_timer;
	float	dt;
	p2List<Module*> list_modules;
	LCG*	random = nullptr;
	
	//Variables needed to show ms and fps graphs
	int	num_fps;
	int	last_frame_ms;
	int	last_fps;
	int frames;
	int capped_ms;
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	LCG& RandomNumber();
	void CapFPS(int max_fps);
private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

// Give App pointer access everywhere
extern Application* App;