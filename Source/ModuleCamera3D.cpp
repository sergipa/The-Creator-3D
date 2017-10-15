#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"
#include "SceneWindow.h"
#include "ModuleScene.h"
#include "PerformanceWindow.h"
#include "Data.h"


ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 3.5f, 10.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);

	name = "Camera";
	can_update = false;
	camera_is_orbital = false;

	key_speed = 38;//DEFAULT LSHIFT
	key_forward = 22;//DEFAULT W
	key_backward = 18;//DEFAULT S
	key_up = 16;//DEFAULT Q
	key_down = 4;//DEFAULT E
	key_left = 0;//DEFAULT A
	key_right = 3;//DEFAULT D
}

ModuleCamera3D::~ModuleCamera3D()
{}
// -----------------------------------------------------------------
bool ModuleCamera3D::Init(Data * editor_config)
{
	CONSOLE_DEBUG("Setting up the camera");
	bool ret = true;

	editor_config->EnterSection("Camera_Config");

	key_speed = App->input->StringToKey(editor_config->GetString("key_speed"));
	key_forward = App->input->StringToKey(editor_config->GetString("key_forward"));
	key_backward = App->input->StringToKey(editor_config->GetString("key_backward"));
	key_up = App->input->StringToKey(editor_config->GetString("key_up"));
	key_down = App->input->StringToKey(editor_config->GetString("key_down"));
	key_left = App->input->StringToKey(editor_config->GetString("key_left"));
	key_right = App->input->StringToKey(editor_config->GetString("key_right"));

	editor_config->LeaveSection();
	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	CONSOLE_DEBUG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	ms_timer.Start();
	if (can_update)
	{
		// Implement a debug camera with keys and mouse
		// Now we can make this movememnt frame rate independant!

		vec3 newPos(0, 0, 0);
		float speed = 20.0f * dt;
		if (App->input->GetKey(key_speed) == KEY_REPEAT)
			speed = 70.0f * dt;

		if (App->input->GetKey(key_up) == KEY_REPEAT) newPos.y += speed;
		if (App->input->GetKey(key_down) == KEY_REPEAT) newPos.y -= speed;

		if (App->input->GetKey(key_forward) == KEY_REPEAT) newPos -= Z * speed;
		if (App->input->GetKey(key_backward) == KEY_REPEAT) newPos += Z * speed;
		if (App->input->GetMouseZ() > 0) newPos -= Z * speed;
		if (App->input->GetMouseZ() < 0) newPos += Z * speed;

		if (App->input->GetKey(key_left) == KEY_REPEAT) newPos -= X * speed;
		if (App->input->GetKey(key_right) == KEY_REPEAT) newPos += X * speed;

		Position += newPos;
		Reference += newPos;

		// Mouse motion ----------------

		if ((App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && !camera_is_orbital) || App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && camera_is_orbital)
		{
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			float Sensitivity = 0.25f;

			Position -= Reference;

			if (dx != 0)
			{
				float DeltaX = (float)dx * Sensitivity;

				X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			}

			if (dy != 0)
			{
				float DeltaY = (float)dy * Sensitivity;

				Y = rotate(Y, DeltaY, X);
				Z = rotate(Z, DeltaY, X);

				if (Y.y < 0.0f)
				{
					Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
					Y = cross(Z, X);
				}
			}

			Position = Reference + Z * length(Position);
		}
		
		// Recalculate matrix -------------
		CalculateViewMatrix();
	}
	App->editor->performance_window->AddModuleData(this->name, ms_timer.ReadMs());
	
	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}

void ModuleCamera3D::FocusOnObject(const vec3 & object, const float & distance)
{
	Reference = object;
	Position = Reference + (Z * distance);
	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

void ModuleCamera3D::SetOrbital(bool is_orbital)
{
	camera_is_orbital = is_orbital;
}

bool ModuleCamera3D::IsOrbital() const
{
	return camera_is_orbital;
}

void ModuleCamera3D::SaveData(Data * data)
{
	data->CreateSection("Camera_Config");

	data->AddString("key_speed", App->input->KeyToString(key_speed));
	data->AddString("key_forward", App->input->KeyToString(key_forward));
	data->AddString("key_backward", App->input->KeyToString(key_backward));
	data->AddString("key_up", App->input->KeyToString(key_up));
	data->AddString("key_down", App->input->KeyToString(key_down));
	data->AddString("key_left", App->input->KeyToString(key_left));
	data->AddString("key_right", App->input->KeyToString(key_right));

	data->CloseSection();
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}