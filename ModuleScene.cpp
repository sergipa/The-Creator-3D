#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"

#include <gl/GL.h>
#include <gl/GLU.h>
#include "glut/glut.h"

#pragma comment (lib, "glut/glut32.lib")

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

	ball1.pos = { 50,0,0 };
	ball1.r = 10;
	ball2.pos = { -50,0,0 };
	ball2.r = 10;

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
	ball1.Translate({-0.1f,0,0});
	ball2.Translate({ 0.1f,0,0 });

	float distance = ball1.Distance(ball2);
	
	if (ball1.Intersects(ball2)) {
		App->imgui->AddLogToConsole("Ball 1 and ball 2 are intersecting");
	}
	else {
		std::string str = std::to_string(distance);
		App->imgui->AddLogToConsole(("Distance between both speheres: " + str).c_str());
	}
	pl.Render();

	return UPDATE_CONTINUE;
}

void ModuleScene::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}