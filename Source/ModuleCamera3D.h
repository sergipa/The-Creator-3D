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

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();
	void SetOrbital(bool is_orbital);
	bool IsOrbital() const;

private:
	void CalculateViewMatrix();
	mat4x4 ViewMatrix, ViewMatrixInverse;
	bool isPlaying = false;
	bool camera_is_orbital;

public:
	vec3 X, Y, Z, Position, Reference;
	bool can_update;
};