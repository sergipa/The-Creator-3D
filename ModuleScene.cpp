#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start()
{
	CONSOLE_LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	pl.normal.x = 0;
	pl.normal.y = 1;
	pl.normal.z = 0;
	pl.constant = 0;
	pl.axis = true;

	ball1.pos = { 0,0,0 };
	ball1.r = 30;
	ball2.pos = { 0,0,0 };
	ball2.r = 30;

	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	CONSOLE_LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleScene::Update(float dt)
{
	/*float ball1_pos = ball1.pos.x - 0.01f;
	ball1.Translate({ball1_pos,0,0});

	float ball2_pos = ball2.pos.x + 0.01f;
	ball2.Translate({ ball2_pos,0,0 });*/

	if (ball1.Intersects(ball2)) {
		CONSOLE_LOG("Ball 1 and ball 2 are intersecting");
		App->imgui->AddLogToConsole("Ball 1 and ball 2 are intersecting");
	}
	pl.Render();

	return UPDATE_CONTINUE;
}

void ModuleScene::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}