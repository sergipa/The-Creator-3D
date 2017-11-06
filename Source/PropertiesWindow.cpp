#include "PropertiesWindow.h"
#include "Application.h"
#include "GameObject.h"
#include "MathGeoLib\MathGeoLib.h"
#include "ComponentCamera.h"
#include "ComponentMeshRenderer.h"
#include "ComponentTransform.h"
#include "TagsAndLayers.h"
#include "ModuleEditor.h"
#include "TagsAndLayersWindow.h"
#include "ModuleScene.h"

PropertiesWindow::PropertiesWindow()
{
	active = true;
	window_name = "Properties";
}

PropertiesWindow::~PropertiesWindow()
{
}

void PropertiesWindow::DrawWindow()
{
	if (ImGui::BeginDock(window_name.c_str(), false, false/*, App->IsPlaying()*/, ImGuiWindowFlags_HorizontalScrollbar)) {
		GameObject* selected_gameobject = nullptr;
		if (App->scene->selected_gameobjects.size() == 1) {
			selected_gameobject = App->scene->selected_gameobjects.front();
		}
		else if (App->scene->selected_gameobjects.size() > 1) {
			ImGui::Text("More than 1 GameObject selected!");
		}

		if (selected_gameobject != nullptr && !selected_gameobject->is_on_destroy) {
			ImGui::Text("Name: %s", selected_gameobject->GetName().c_str());
			bool is_gameobject_active = selected_gameobject->IsActive();

			if (ImGui::Checkbox("Active", &is_gameobject_active)) {
				selected_gameobject->SetActive(is_gameobject_active);
			}
			ImGui::SameLine();
			bool is_static = selected_gameobject->IsStatic();
			if (ImGui::Checkbox("Static", &is_static))
			{
				selected_gameobject->SetStatic(is_static);
			}
			ImGui::Text("Tag:");
			ImGui::SameLine();
			if (ImGui::SmallButton((selected_gameobject->GetTag() + "##tags").c_str())) {
				ImGui::OpenPopup("Tags##gameobject");
			}
			if (ImGui::BeginPopup("Tags##gameobject")) {
				for (int i = 0; i < App->tags_and_layers->tags_list.size(); i++) {
					std::string name = App->tags_and_layers->tags_list[i];
					if (ImGui::MenuItem(name.c_str())) {
						selected_gameobject->SetTag(name);
					}
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Add Tag##gameobject")) {
					App->editor->tags_and_layers_window->active = true;
				}
				ImGui::EndPopup();
			}
			ImGui::SameLine();
			ImGui::Text("Layer:");
			ImGui::SameLine();
			if (ImGui::SmallButton((selected_gameobject->GetLayer() + "##layers").c_str())) {
				ImGui::OpenPopup("Layers##gameobject");
			}
			if (ImGui::BeginPopup("Layers##gameobject")) {
				for (int i = 0; i < App->tags_and_layers->layers_list.size(); i++) {
					std::string name = App->tags_and_layers->layers_list[i];
					if (ImGui::MenuItem(name.c_str())) {
						selected_gameobject->SetLayer(name);
					}
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Add Layer##gameobject")) {
					App->editor->tags_and_layers_window->active = true;
				}
				ImGui::EndPopup();
			}
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			for (std::list<Component*>::iterator it = selected_gameobject->components_list.begin(); it != selected_gameobject->components_list.end(); it++) {
				DrawComponent((*it));
				ImGui::Separator();
				ImGui::Spacing();
			}

			if (ImGui::Button("Add Component")) {
				ImGui::OpenPopup("Components");
			}
			if (ImGui::BeginPopup("Components"))
			{
				if (ImGui::MenuItem("Mesh Renderer")) {
					if (selected_gameobject->GetComponent(Component::MeshRenderer) == nullptr) {
						selected_gameobject->AddComponent(Component::MeshRenderer);
					}
					else
					{
						CONSOLE_WARNING("GameObject can't have more than 1 Mesh Renderer!");
					}
				}
				if (ImGui::MenuItem("Camera")) {
					if (selected_gameobject->GetComponent(Component::Camera) == nullptr) {
						selected_gameobject->AddComponent(Component::Camera);
					}
					else
					{
						CONSOLE_WARNING("GameObject can't have more than 1 Camera!");
					}
				}
				ImGui::EndPopup();
			}
		}
	}

	ImGui::EndDock();
}

void PropertiesWindow::DrawComponent(Component * component)
{
	switch (component->GetType())
	{
	case Component::Transform:
		DrawTransformPanel((ComponentTransform*)component);
		break;
	case Component::Camera:
		DrawCameraPanel((ComponentCamera*)component);
		break;
	case Component::RigidBody:
		break;
	case Component::MeshRenderer:
		DrawMeshRendererPanel((ComponentMeshRenderer*)component);
		break;
	case Component::BoxCollider:
		break;
	case Component::CircleCollider:
		break;
	case Component::AudioSource:
		break;
	case Component::Animaton:
		break;
	case Component::Script:
		break;
	case Component::ParticleSystem:
		break;
	default:
		break;
	}
}

void PropertiesWindow::DrawTransformPanel(ComponentTransform * transform)
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
		float3 position;
		float3 rotation;
		float3 scale;
		if (transform->GetGameObject()->IsRoot()) {
			position = transform->GetGlobalPosition();
			rotation = transform->GetGlobalRotation();
			scale = transform->GetGlobalScale();
		}
		else {
			position = transform->GetLocalPosition();
			rotation = transform->GetLocalRotation();
			scale = transform->GetLocalScale();
		}
		bool is_static = !transform->GetGameObject()->IsStatic();
		if (ImGui::DragFloat3("Position", (float*)&position, is_static, 0.25f)) {
			transform->SetPosition(position);
		}
		if (ImGui::DragFloat3("Rotation", (float*)&rotation, is_static, 0.25f, -360, 360)) {
			transform->SetRotation(rotation);
		}
		if (ImGui::DragFloat3("Scale", (float*)&scale, is_static, 0.25f)) {
			transform->SetScale(scale);
		}
	}
}

void PropertiesWindow::DrawMeshRendererPanel(ComponentMeshRenderer * mesh_renderer)
{
	if (ImGui::CollapsingHeader("Mesh Renderer", ImGuiTreeNodeFlags_DefaultOpen)) {
		bool is_active = mesh_renderer->IsActive();
		if (ImGui::Checkbox("Active##Mesh_Renderer", &is_active))
		{
			mesh_renderer->SetActive(is_active);
		}
		if(ImGui::TreeNodeEx("Mesh Info", ImGuiTreeNodeFlags_OpenOnArrow))
		{
			if (mesh_renderer->GetMesh() == nullptr)
			{
				ImGui::TextColored(ImVec4(1, 0, 0, 1), "Mesh not found");
				ImGui::TreePop();
				return;
			}
			ImGui::Text("Triangle Count: %d", mesh_renderer->GetMesh()->num_indices / 3);
			ImGui::Text("Vertex Count: %d", mesh_renderer->GetMesh()->num_vertices);
			ImGui::Text("Indices Count: %d", mesh_renderer->GetMesh()->num_indices);
			ImGui::Text("Normals: "); ImGui::SameLine(); (mesh_renderer->GetMesh()->id_normals > 0) ? ImGui::TextColored(ImVec4(0, 1, 0, 1), ("yes")) : ImGui::TextColored(ImVec4(1, 0, 0, 1), ("no"));
			ImGui::Text("Colors: "); ImGui::SameLine(); (mesh_renderer->GetMesh()->id_colors > 0) ? ImGui::TextColored(ImVec4(0, 1, 0, 1), ("yes")) : ImGui::TextColored(ImVec4(1, 0, 0, 1), ("no"));
			ImGui::Text("UV: "); ImGui::SameLine(); (mesh_renderer->GetMesh()->id_texture_coords > 0) ? ImGui::TextColored(ImVec4(0, 1, 0, 1), ("yes")) : ImGui::TextColored(ImVec4(1, 0, 0, 1), ("no"));
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("Texture Info", ImGuiTreeNodeFlags_OpenOnArrow))
		{
			if (mesh_renderer->GetTexture() == nullptr)
			{
				ImGui::TextColored(ImVec4(1, 0, 0, 1), "Not using a Texture");
				ImGui::TreePop();
				return;
			}
			ImGui::Text("Texture ID: %d", mesh_renderer->GetTexture()->GetID());
			ImGui::Text("Texture Path: %s", mesh_renderer->GetTexture()->GetAssetsPath().c_str());
			if (ImGui::IsItemHoveredRect() && ImGui::CalcTextSize(("Texture Path: " + mesh_renderer->GetTexture()->GetAssetsPath()).c_str()).x > ImGui::GetContentRegionAvailWidth()) {
				ImGui::BeginTooltip();
				ImGui::Text("%s", mesh_renderer->GetTexture()->GetAssetsPath().c_str());
				ImGui::EndTooltip();
			}
			ImGui::Text("Texture Name: %s", mesh_renderer->GetTexture()->GetName().c_str());
			if (ImGui::IsItemHoveredRect() && ImGui::CalcTextSize(("Texture Name: " + mesh_renderer->GetTexture()->GetName()).c_str()).x > ImGui::GetContentRegionAvailWidth()) {
				ImGui::BeginTooltip();
				ImGui::Text("%s", mesh_renderer->GetTexture()->GetName().c_str());
				ImGui::EndTooltip();
			}
			ImGui::Text("Texture Size: %d x %d", mesh_renderer->GetTexture()->GetWidth(), mesh_renderer->GetTexture()->GetHeight());
			ImGui::Text("Texture Format: %s", mesh_renderer->GetTexture()->GetFormatString().c_str());
			ImGui::Text("Texture Type: %s", mesh_renderer->GetTexture()->GetTypeString().c_str());
			ImGui::TreePop();
		}

		ImGui::Spacing();
	}
}

void PropertiesWindow::DrawCameraPanel(ComponentCamera * camera)
{
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
		bool is_active = camera->IsActive();
		if (ImGui::Checkbox("Active##Camera", &is_active))
		{
			camera->SetActive(is_active);
		}

		Color background_color = camera->GetBackgroundColor();
		ImGui::Text("Background Color:");
		if (ImGui::ColorEdit4("##Background Color", &background_color.r))
		{
			camera->SetBackgroundColor(background_color);
		}
		ImGui::Text("Culling Mask:");
		if (ImGui::Button("Select Layers")) {
			ImGui::OpenPopup("Layers##camera");
		}
		if (ImGui::BeginPopup("Layers##camera")) {
			for (int i = 0; i < App->tags_and_layers->layers_list.size(); i++) {
				std::string name = App->tags_and_layers->layers_list[i];
				bool selected = false;
				if (std::find(camera->layers_to_draw.begin(), camera->layers_to_draw.end(), name) != camera->layers_to_draw.end()) selected = true;
				if (ImGui::MenuItem(name.c_str(), "", selected)) {
					if (selected) camera->RemoveLayerToDraw(name);
					else camera->AddLayerToDraw(name);
				}
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Add Layer##camera")) {
				App->editor->tags_and_layers_window->active = true;
			}
			ImGui::EndPopup();
		}
		float fov = camera->GetFOV();
		ImGui::Text("Field of View:");
		if (ImGui::SliderFloat("##Field of View", &fov, 1, 160))
		{
			camera->SetFOV(fov);
		}
		float near_plane = camera->GetNearPlaneDistance();
		float far_plane = camera->GetFarPlanceDistance();
		ImGui::Text("Near Plane:");
		if (ImGui::DragFloat("##Near Plane", &near_plane, true, 0.025f, 0.01, far_plane - 0.1f))
		{
			camera->SetNearPlaneDistance(near_plane);
		}
		ImGui::Text("Far Plane:");
		if (ImGui::DragFloat("##Far Plane", &far_plane, true, 0.025f, near_plane + 0.1f))
		{
			camera->SetFarPlaneDistance(far_plane);
		}
		Rect viewport = camera->GetViewport();
		ImGui::Text("Viewport:");
		if (ImGui::DragInt("X", &viewport.left, true, 0.025f, 0, 1))
		{
			camera->SetViewport(viewport);
		}
		if (ImGui::DragInt("Y", &viewport.top, true, 0.025f, 0, 1))
		{
			camera->SetViewport(viewport);
		}
		if (ImGui::DragInt("W", &viewport.right, true, 0.025f, 1, 0))
		{
			camera->SetViewport(viewport);
		}
		if (ImGui::DragInt("H", &viewport.bottom, true, 0.025f, 1, 0))
		{
			camera->SetViewport(viewport);
		}
		int render_order = camera->GetRenderOrder();
		ImGui::Text("Render Order:");
		if (ImGui::DragInt("##Render Order", &render_order))
		{
			camera->SetRenderOrder(render_order);
		}
		
		ImGui::Spacing();
	}
}
