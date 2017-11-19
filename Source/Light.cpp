#include "Globals.h"
#include "Light.h"
#include "OpenGL.h"

Light::Light() : ref(-1), on(false), position(0.0f, 0.0f, 0.0f, 0.0f)
{}

void Light::Init()
{
	glLightfv(ref, GL_AMBIENT, &ambient);
	glLightfv(ref, GL_DIFFUSE, &diffuse);
	//glLightfv(ref, GL_SPECULAR, &specular);
}

void Light::SetPos(float x, float y, float z, float w)
{
	position.x = x;
	position.y = y;
	position.z = z;
	position.w = w;
}

void Light::Render()
{
	if(on)
	{
		float pos[] = {position.x, position.y, position.z, position.w};
		glLightfv(ref, GL_POSITION, pos);
	}
}

void Light::Active(bool active)
{
	if (active)
	{
		glEnable(ref);
	}
	else {
		glDisable(ref);
	}
	on = active;
}