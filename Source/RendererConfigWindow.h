#pragma once
#include "Window.h"

class RendererConfigWindow :
	public Window
{
public:
	RendererConfigWindow();
	virtual ~RendererConfigWindow();

	void DrawWindow();
	void SetDefaultValues();

private:
	float fog_color[4];
	float fog_density;
	float light_ambient[4];
	float light_diffuse[4];
	float light_specular[4];
	float light_position[4];
	float light_constant_attenuation;
	float light_linear_attenuation;
	float light_quadratic_attenuation;
	float light_spot_direction[3];
};

