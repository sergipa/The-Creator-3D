#include "Application.h"
#include "Data.h"

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
	physics = new ModulePhysics3D(this);
	editor = new ModuleEditor(this);
	import = new ModuleImport(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(editor);
	AddModule(input);
	AddModule(audio);
	//AddModule(physics);
	AddModule(renderer3D);
	AddModule(scene);
	AddModule(import);

	random = new math::LCG();
	cursor = nullptr;
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

	while (item != list_modules.end() && ret == true)
	{
		ret = (*item)->Init(&data);
		++item;
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
		ret = (*item)->PreUpdate(dt);
		++item;
	}

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->Update(dt);
		++item;
	}

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
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
	data->CloseSection();
	data->SaveAsJSON(EDITOR_CONFIG_FILE);
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}