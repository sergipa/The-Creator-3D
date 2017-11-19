#include "SkyDome.h"
#include "Primitive.h"
#include "Texture.h"
#include "ModuleTextureImporter.h"
#include "Application.h"

SkyDome::SkyDome()
{
	texture = nullptr;
	sphere = nullptr;
}

SkyDome::~SkyDome()
{
	RELEASE(texture);
	RELEASE(sphere);
}

void SkyDome::Create(float radius)
{
	sphere = new pTexturedSphere(radius, texture);
}

void SkyDome::SetTexture(std::string texture_path)
{
	texture = App->texture_importer->LoadTextureFromLibrary(texture_path);
}

void SkyDome::SetTexture(Texture * texture)
{
	sphere->SetTexture(texture);
}

void SkyDome::SetPosition(float3 pos)
{
	sphere->SetPos(pos.x, pos.y, pos.z);
}

void SkyDome::Render()
{
	sphere->Render();
}
