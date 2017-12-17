
#pragma once
#include "Color.h"
#include "MathGeoLib/Math/float3.h"

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
	float3 position;

	int ref;
	bool on;
};