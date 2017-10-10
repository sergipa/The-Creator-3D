#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "OpenGL.h"
#include "ModuleEditor.h"
#include "Primitive.h"
#include "Data.h"
#include "Component.h"
#include "Mesh.h"
#include "ComponentMeshRenderer.h"

#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "Glew/libx86/glew32.lib")

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	use_vsync = true;
	is_using_lightning = false;
	is_using_depth_test = false;
	is_using_cull_test = false;
	is_using_color_material = false;
	is_using_texture2D = false;
	is_using_fog = false;
	name = "Renderer";
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init(Data* editor_config)
{
	CONSOLE_LOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		CONSOLE_LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	glewInit();
	
	if(ret == true)
	{
		//Get Config Data
		editor_config->EnterSection("Renderer_Config");
		use_vsync = editor_config->GetBool("Vsync");
		int MSAA_level = editor_config->GetInt("MSAA_Level");
		editor_config->LeaveSection();
		if (MSAA_level < 0) MSAA_level = 2;

		//Use Vsync
		if(use_vsync && SDL_GL_SetSwapInterval(1) < 0)
			CONSOLE_LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		textureMSAA = new RenderTextureMSAA();
		textureMSAA->Create(App->window->GetWidth(), App->window->GetHeight(), MSAA_level);

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			CONSOLE_LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			CONSOLE_LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.2f, 0.2f, 0.2f, 1.f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			CONSOLE_LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.3f, 0.3f, 0.3f, 1.0f);
		lights[1].diffuse.Set(0.7f, 0.7f, 0.7f, 1.0f);
		lights[0].SetPos(-1.0f, 1.0f, 1.0f);
		lights[0].Init();

		lights[1].ref = GL_LIGHT1;
		lights[1].ambient.Set(0.2f, 0.2f, 0.2f, 1.0f);
		lights[1].diffuse.Set(0.7f, 0.7f, 0.7f, 1.0f);
		lights[1].specular.Set(1.0f, 1.0f, 1.0f, 1.0f);
		lights[1].SetPos(-1.0f, 1.0f, 1.0f);
		lights[1].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		lights[1].Active(true);
		glEnable(GL_COLOR_MATERIAL);
		is_using_lightning = true;
		is_using_depth_test = true;
		is_using_cull_test = true;
		is_using_color_material = true;

		glEnable(GL_MULTISAMPLE);
	}
	//glGenBuffers(1, &)
	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	ms_timer.Start();
	glEnable(GL_LIGHTING);

	textureMSAA->Bind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{

<<<<<<< HEAD
	DrawScene();
=======
	//Test. This should be removed in the future
	pSphere cube;
	cube.color = Red;
	cube.SetPos(0, 0, 0);
	cube.Render();

	pRay ray(0, 0, 0, 0, 10, 0);
	ray.Render();
	//drawTeapot();
	pPlane pl;
	pl.normal.x = 0;
	pl.normal.y = 1;
	pl.normal.z = 0;
	pl.constant = 0;
	pl.axis = true;
	pl.Render();
>>>>>>> origin/master

	textureMSAA->Render();

	textureMSAA->Unbind();
	//EditorUI can't be drawn in wireframe mode!
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//Disable Lighting before draw editor or shadows will appear in menu bars and ligth will affect editor colors.
	glDisable(GL_LIGHTING);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	App->editor->DrawEditor();

	App->editor->SendDataToPerformance(this->name, ms_timer.ReadMs());

	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

void ModuleRenderer3D::DrawScene()
{
	for (std::list<GameObject*>::iterator it = gameobjects_to_draw.begin(); it != gameobjects_to_draw.end(); it++)
	{
		ComponentMeshRenderer* mesh_renderer = (ComponentMeshRenderer*)(*it)->GetComponent(Component::MeshRenderer);
		Mesh* mesh = mesh_renderer->GetMesh();
		
	}
}

void ModuleRenderer3D::AddGameObjectToDraw(GameObject * gameobject)
{
	gameobjects_to_draw.push_back(gameobject);
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	CONSOLE_LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);
	glMatrixMode(GL_MODELVIEW);
}

void ModuleRenderer3D::SetWireframeMode()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void ModuleRenderer3D::SaveData(Data * data)
{
	data->CreateSection("Renderer_Config");
	data->AddBool("Vsync", use_vsync);
	data->AddInt("MSAA_Level", textureMSAA->GetCurrentMSAALevel());
	data->CloseSection();
}

void ModuleRenderer3D::SetActiveLighting(bool active)
{
	is_using_lightning = active;

	if (active)
	{
		for (uint i = 1; i < MAX_LIGHTS; ++i)
			lights[i].Active(true);
	}
	else 
	{
		for (uint i = 1; i < MAX_LIGHTS; ++i)
			lights[i].Active(false);
	}
}

void ModuleRenderer3D::SetActiveDepthTest(bool active)
{
	is_using_depth_test = active;

	if (active)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
}

void ModuleRenderer3D::SetActiveCullTest(bool active)
{
	is_using_cull_test = active;

	if (active)
	{
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
}

void ModuleRenderer3D::SetActiveColorMaterial(bool active)
{
	is_using_color_material = active;

	if (active)
	{
		glEnable(GL_COLOR_MATERIAL);
	}
	else
	{
		glDisable(GL_COLOR_MATERIAL);
	}
}

void ModuleRenderer3D::SetActiveTexture2D(bool active)
{
	is_using_texture2D = active;

	if (active)
	{
		glEnable(GL_TEXTURE_2D);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}
}

void ModuleRenderer3D::SetActiveFog(bool active)
{
	is_using_fog = active;

	if (active)
	{
		glEnable(GL_FOG);
	}
	else
	{
		glDisable(GL_FOG);
	}
}

bool ModuleRenderer3D::GetActiveLighting() const
{
	return is_using_lightning;
}

bool ModuleRenderer3D::GetActiveDepthTest() const
{
	return is_using_depth_test;
}

bool ModuleRenderer3D::GetActiveCullTest() const
{
	return is_using_cull_test;
}

bool ModuleRenderer3D::GetActiveColorMaterial() const
{
	return is_using_color_material;
}

bool ModuleRenderer3D::GetActiveTexture2D() const
{
	return is_using_texture2D;
}

bool ModuleRenderer3D::GetActiveFog() const
{
	return is_using_fog;
}

void ModuleRenderer3D::EnableTestLight()
{
	lights[7].ref = GL_LIGHT7;
	lights[7].Active(true);
	for (uint i = 1; i < MAX_LIGHTS - 1; ++i)
		lights[i].Active(false);
}

void ModuleRenderer3D::DisableTestLight()
{
	lights[7].Active(false);
	for (uint i = 1; i < MAX_LIGHTS - 1; ++i)
		lights[i].Active(true);
}


