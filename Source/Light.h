
#pragma once
#include "Color.h"
#include "glmath.h"

struct Light
{
	Light();

	void Init();
	void SetPos(float x, float y, float z, float w = 0.0f);
	void Active(bool active);
	void Render();

	Color ambient;
	Color diffuse;
	Color specular;
	vec4 position;

	int ref;
	bool on;
};