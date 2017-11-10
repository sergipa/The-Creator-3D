#include "Application.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModuleEditor.h"
#include "ModuleResources.h"
#include "ModuleTime.h"
#include "ModuleFileSystem.h"
#include "ModuleMeshImporter.h"
#include "ModuleTextureImporter.h"
#include "ModulePrefabImporter.h"
#include "Data.h"
#include "TagsAndLayers.h"

Application::Application()
{
	frames = 0;
	last_frame_ms = -1;
	last_fps = 0;
	num_fps = 0;
	capped_ms = 1000 / 60;

	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	scene = new ModuleScene(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	//physics = new ModulePhysics3D(this);
	editor = new ModuleEditor(this);
	resources = new ModuleResources(this);
	time = new ModuleTime(this);
	file_system = new ModuleFileSystem(this);
	mesh_importer = new ModuleMeshImporter(this);
	texture_importer = new ModuleTextureImporter(this);
	prefab_importer = new ModulePrefabImporter(this);
	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(file_system);
	AddModule(window);
	AddModule(input);
	AddModule(audio);
	AddModule(renderer3D);
	AddModule(camera);
	AddModule(scene);
	AddModule(mesh_importer);
	AddModule(texture_importer);
	AddModule(prefab_importer);
	AddModule(editor);
	AddModule(resources);
	//TIME
	AddModule(time);
	//Game Modules
	//AddModule(physics);

	random = new math::LCG();
	cursor = nullptr;

	tags_and_layers = new TagsAndLayers();
}

Application::~Application()
{
	std::list<Module*>::iterator item = list_modules.begin();

	while (item != list_modules.end())
	{
		delete *item;
		++item;
	}

	list_modules.clear();

	RELEASE(random);
	SDL_FreeCursor(cursor);
}

bool Application::Init()
{
	bool ret = true;

	Data data;

	if (!data.LoadJSON(EDITOR_CONFIG_FILE))
	{
		CreateEngineData(&data);
	}

	// Call Init() in all modules
	std::list<Module*>::iterator item = list_modules.begin();

	if (data.EnterSection("Engine_Settings"))
	{
		while (item != list_modules.end() && ret == true)
		{
			ret = (*item)->Init(&data);
			++item;
		}
		data.LeaveSection();
	}

	// After all Init calls we call Start() in all modules
	CONSOLE_DEBUG("-------------- Application Start --------------");
	item = list_modules.begin();

	while (item != list_modules.end() && ret == true)
	{
		ret = (*item)->Start();
		++item;
	}
	
	ms_timer.Start();
	fps_timer.Start();

	tags_and_layers->Load(&data);

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	frames++;
	num_fps++;

	if (fps_timer.Read() >= 1000)//in ms
	{
		fps_timer.Start();
		last_fps = num_fps;
		num_fps = 0;
	}

	last_frame_ms = ms_timer.Read();
	
	if (capped_ms > 0 && last_frame_ms < capped_ms)
	{
		SDL_Delay(capped_ms - last_frame_ms);
	}

	App->editor->AddData_Editor(last_frame_ms, last_fps);
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	std::list<Module*>::iterator item = list_modules.begin();
	
	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		if((*item)->is_game)
			ret = (*item)->PreUpdate(time->GetGameDt());
		else
			ret = (*item)->PreUpdate(dt);

		++item;
	}

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		if ((*item)->is_game)
			ret = (*item)->Update(time->GetGameDt());
		else
			ret = (*item)->Update(dt);

		++item;
	}

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		if ((*item)->is_game)
			ret = (*item)->PostUpdate(time->GetGameDt());
		else
			ret = (*item)->PostUpdate(dt);

		++item;
	}
	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	Data data;
	CreateEngineData(&data);

	for (std::list<Module*>::iterator it = list_modules.begin(); it != list_modules.end() && ret == true; ++it)
	{
		ret = (*it)->CleanUp();
	}

	return ret;
}

LCG & Application::RandomNumber()
{
	return *random;
}

void Application::CapFPS(int max_fps)
{
	if (max_fps > 0) capped_ms = 1000 / max_fps;
		
	else capped_ms = 0;
		
}

void Application::SetCursor(SDL_SystemCursor id)
{
	cursor = SDL_CreateSystemCursor(id);
	SDL_SetCursor(cursor);
}

void Application::CreateEngineData(Data * data)
{
	data->CreateSection("Engine_Settings");
	for (std::list<Module*>::iterator it = list_modules.begin(); it != list_modules.end(); ++it)
	{
		(*it)->SaveData(data);
	}
	tags_and_layers->Save(data);
	data->CloseSection();
	data->SaveAsJSON(EDITOR_CONFIG_FILE);
}

void Application::UpdateStep()
{
	update_status ret = UPDATE_CONTINUE;

	std::list<Module*>::iterator item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		if ((*item)->is_game)
			ret = (*item)->PreUpdate(dt);

		++item;
	}

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		if ((*item)->is_game)
			ret = (*item)->Update(dt);

		++item;
	}

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		if ((*item)->is_game)
			ret = (*item)->PostUpdate(dt);

		++item;
	}
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}