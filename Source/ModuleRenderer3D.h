#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "RenderTextureMSAA.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init(Data* editor_config = nullptr);
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);
	void SetWireframeMode();
	void SaveData(Data* data);

	void SetActiveLighting(bool active);
	void SetActiveDepthTest(bool active);
	void SetActiveCullTest(bool active);
	void SetActiveColorMaterial(bool active);
	void SetActiveTexture2D(bool active);
	void SetActiveFog(bool active);

	bool GetActiveLighting() const;
	bool GetActiveDepthTest() const;
	bool GetActiveCullTest() const;
	bool GetActiveColorMaterial() const;
	bool GetActiveTexture2D() const;
	bool GetActiveFog() const;

	void EnableTestLight();
	void DisableTestLight();


public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
	RenderTextureMSAA* textureMSAA;
private:
	bool use_vsync;
	bool is_using_lightning;
	bool is_using_depth_test;
	bool is_using_cull_test;
	bool is_using_color_material;
	bool is_using_texture2D;
	bool is_using_fog;
};