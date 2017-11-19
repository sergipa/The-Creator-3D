#include "ResourcesConfigWindow.h"
#include "Application.h"
#include "ModuleResources.h"
#include <map>
#include "Mesh.h"
#include "GameObject.h"
#include "Prefab.h"
#include "Texture.h"
#include "Material.h"

ResourcesConfigWindow::ResourcesConfigWindow()
{
	active = false;
	window_name = "Resource Management";
}

ResourcesConfigWindow::~ResourcesConfigWindow()
{
}

void ResourcesConfigWindow::DrawWindow()
{
	ImGui::Begin(window_name.c_str(), &active,
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_ShowBorders);

	if (ImGui::TreeNodeEx("Meshes", ImGuiTreeNodeFlags_OpenOnArrow))
	{
		std::map<uint, Mesh*> meshes = App->resources->GetMeshesList();
		for (std::map<uint, Mesh*>::iterator it = meshes.begin(); it != meshes.end(); it++)
		{
			ImGui::BulletText(it->second->GetName().c_str());
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0,1, 1, 1), "- Scene Gameobjects using it:");
			ImGui::SameLine();
			ImVec4 color = it->second->GetUsedCount() > 0 ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1);
			ImGui::TextColored(color, "%d", it->second->GetUsedCount());
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNodeEx("Textures", ImGuiTreeNodeFlags_OpenOnArrow))
	{
		std::map<uint, Texture*> textures = App->resources->GetTexturesList();
		for (std::map<uint, Texture*>::const_iterator it = textures.begin(); it != textures.end(); it++)
		{
			ImGui::BulletText(it->second->GetName().c_str());
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0, 1, 1, 1), "- Scene Gameobjects using it:");
			ImGui::SameLine();
			ImVec4 color = it->second->GetUsedCount() > 0 ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1);
			ImGui::TextColored(color, "%d", it->second->GetUsedCount());
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNodeEx("GameObjects", ImGuiTreeNodeFlags_OpenOnArrow))
	{
		std::map<uint, GameObject*> gameobjects = App->resources->GetGameobjectsList();
		for (std::map<uint, GameObject*>::const_iterator it = gameobjects.begin(); it != gameobjects.end(); it++)
		{
			ImGui::BulletText(it->second->GetName().c_str());
			ImGui::SameLine();
			//ImGui::Text("Used in scene: %d", it->second->GetUsedCount());
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNodeEx("Prefabs", ImGuiTreeNodeFlags_OpenOnArrow))
	{
		std::map<uint, Prefab*> prefabs = App->resources->GetPrefabsList();
		for (std::map<uint, Prefab*>::const_iterator it = prefabs.begin(); it != prefabs.end(); it++)
		{
			ImGui::BulletText(it->second->GetName().c_str());
			ImGui::SameLine();
			//ImGui::Text("Scene Gameobjects using it: %d", it->second->GetUsedCount());
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNodeEx("Material", ImGuiTreeNodeFlags_OpenOnArrow))
	{
		std::map<uint, Material*> materials = App->resources->GetMaterialsList();
		for (std::map<uint, Material*>::const_iterator it = materials.begin(); it != materials.end(); it++)
		{
			ImGui::BulletText(it->second->GetName().c_str());
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0, 1, 1, 1), "- Scene Gameobjects using it:");
			ImGui::SameLine();
			ImVec4 color = it->second->GetUsedCount() > 0 ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1);
			ImGui::TextColored(color, "%d", it->second->GetUsedCount());
		}
		ImGui::TreePop();
	}

	ImGui::End();
}
