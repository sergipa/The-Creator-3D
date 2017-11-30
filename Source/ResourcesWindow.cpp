#include "ResourcesWindow.h"
#include "Application.h"
#include "ModuleResources.h"
#include "Texture.h"
#include "Mesh.h"
#include "Prefab.h"
#include "Material.h"
#include "GameObject.h"

ResourcesWindow::ResourcesWindow()
{
	active = false;
	window_name = "Resources";

	texture_to_return = nullptr;
	mesh_to_return = nullptr;
	prefab_to_return = nullptr;
	gameobject_to_return = nullptr;
	material_to_return = nullptr;
	script_to_return = nullptr;

	texture_changed = false;
	mesh_changed = false;
	prefab_changed = false;
	gameobject_changed = false;
	material_changed = false;
	script_changed = false;

	type = Resource::Unknown;
}

ResourcesWindow::~ResourcesWindow()
{
}

void ResourcesWindow::DrawWindow()
{
	ImGui::SetNextWindowSize({ 300,400 });
	ImGui::Begin(window_name.c_str(), &active,
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_ShowBorders);

	switch (type)
	{
	case Resource::TextureResource:
		textures_list = App->resources->GetTexturesList();
		if (ImGui::Selectable("None##Texture"))
		{
			texture_to_return = nullptr;
			texture_changed = true;
			break;
		}
		for (std::map<uint, Texture*>::const_iterator it = textures_list.begin(); it != textures_list.end(); it++)
		{
			if (ImGui::Selectable(it->second->GetName().c_str()))
			{
				texture_to_return = it->second;
				texture_changed = true;
				break;
			}
		}
		break;
	case Resource::MeshResource:
		meshes_list = App->resources->GetMeshesList();
		if (ImGui::Selectable("None##Mesh"))
		{
			mesh_to_return = nullptr;
			mesh_changed = true;
			break;
		}
		for (std::map<uint, Mesh*>::const_iterator it = meshes_list.begin(); it != meshes_list.end(); it++)
		{
			if (ImGui::Selectable(it->second->GetName().c_str()))
			{
				mesh_to_return = it->second;
				mesh_changed = true;
				break;
			}
		}
		break;
	case Resource::SceneResource:
		break;
	case Resource::AnimationResource:
		break;
	case Resource::PrefabResource:
		prefabs_list = App->resources->GetPrefabsList();
		if (ImGui::Selectable("None##prefab"))
		{
			prefab_to_return = nullptr;
			prefab_changed = true;
			break;
		}
		for (std::map<uint, Prefab*>::const_iterator it = prefabs_list.begin(); it != prefabs_list.end(); it++)
		{
			if (ImGui::Selectable(it->second->GetName().c_str()))
			{
				prefab_to_return = it->second;
				prefab_changed = true;
				break;
			}
		}
		break;
	case Resource::ScriptResource:
		scripts_list = App->resources->GetScriptsList();
		if (ImGui::Selectable("None##script"))
		{
			prefab_to_return = nullptr;
			prefab_changed = true;
			break;
		}
		for (std::map<uint, Prefab*>::const_iterator it = prefabs_list.begin(); it != prefabs_list.end(); it++)
		{
			if (ImGui::Selectable(it->second->GetName().c_str()))
			{
				prefab_to_return = it->second;
				prefab_changed = true;
				break;
			}
		}
		break;
	case Resource::AudioResource:
		break;
	case Resource::ParticleFXResource:
		break;
	case Resource::FontResource:
		break;
	case Resource::RenderTextureResource:
		break;
	case Resource::GameObjectResource:
		gameobjects_list = App->resources->GetGameobjectsList();
		if (ImGui::Selectable("None##gameobject"))
		{
			gameobject_to_return = nullptr;
			gameobject_changed = true;
			break;
		}
		for (std::map<uint, GameObject*>::const_iterator it = gameobjects_list.begin(); it != gameobjects_list.end(); it++)
		{
			if (ImGui::Selectable(it->second->GetName().c_str()))
			{
				gameobject_to_return = it->second;
				gameobject_changed = true;
				break;
			}
		}
		break;
	case Resource::MaterialResource:
		materials_list = App->resources->GetMaterialsList();
		if (ImGui::Selectable("None##material"))
		{
			material_to_return = nullptr;
			material_changed = true;
			break;
		}
		for (std::map<uint, Material*>::const_iterator it = materials_list.begin(); it != materials_list.end(); it++)
		{
			if (ImGui::Selectable(it->second->GetName().c_str()))
			{
				material_to_return = it->second;
				material_changed = true;
				break;
			}
		}
		break;
	case Resource::Unknown:
		break;
	default:
		break;
	}

	ImGui::End();
}

void ResourcesWindow::SetResourceType(Resource::ResourceType type)
{
	this->type = type;
}

Texture * ResourcesWindow::GetTexture() const
{
	return texture_to_return;
}

Mesh * ResourcesWindow::GetMesh() const
{
	return mesh_to_return;
}

Prefab * ResourcesWindow::GetPrefab() const
{
	return prefab_to_return;
}

GameObject * ResourcesWindow::GetGameobject() const
{
	return gameobject_to_return;
}

Material * ResourcesWindow::GetMaterial() const
{
	return material_to_return;
}

Script * ResourcesWindow::GetScript() const
{
	return script_to_return;
}

void ResourcesWindow::Reset()
{
	texture_changed = false;
	mesh_changed = false;
	prefab_changed = false;
	gameobject_changed = false;
	material_changed = false;
	script_changed = false;

	texture_to_return = nullptr;
	mesh_to_return = nullptr;
	prefab_to_return = nullptr;
	gameobject_to_return = nullptr;
	material_to_return = nullptr;
	script_to_return = nullptr;
}
