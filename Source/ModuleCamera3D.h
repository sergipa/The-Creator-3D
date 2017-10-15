#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "ModuleEditor.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Init(Data* editor_config);
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void FocusOnObject(const vec3& object, const float& distance);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();
	void SetOrbital(bool is_orbital);
	bool IsOrbital() const;

	void SaveData(Data* data);
private:
	void CalculateViewMatrix();
	mat4x4 ViewMatrix, ViewMatrixInverse;
	bool isPlaying = false;
	bool camera_is_orbital;
public:
	vec3 X, Y, Z, Position, Reference;
	bool can_update;

	int key_speed;
	int key_forward;
	int key_backward;
	int key_up;
	int key_down;
	int key_left;
	int key_right;
};