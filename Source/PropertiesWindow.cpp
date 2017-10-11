#include "PropertiesWindow.h"
#include "Application.h"
#include "GameObject.h"
#include "MathGeoLib\Math\float3.h"

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

		if (selected_gameobject != nullptr) {
			bool is_gameobject_active = selected_gameobject->IsActive();
			ImGui::SameLine();
			if (ImGui::Checkbox("", &is_gameobject_active)) {
				selected_gameobject->SetActive(is_gameobject_active);
			}
			ImGui::SameLine();
			ImGui::Text("Name: %s", selected_gameobject->GetName().c_str());
			ImGui::Text("Tag:");
			ImGui::SameLine();
			if (ImGui::SmallButton((selected_gameobject->GetTag() + "##tags").c_str())) {
				ImGui::OpenPopup("Tags");
			}
			if (ImGui::BeginPopup("Tags")) {
				//This should be the tags! now is a test
				for (int i = 0; i < 5; i++) {
					std::string name = "tag" + std::to_string(i);
					if (ImGui::MenuItem(name.c_str())) {
						selected_gameobject->SetTag(name);
					}
				}
				ImGui::EndPopup();
			}
			ImGui::SameLine();
			ImGui::Text("Layer:");
			ImGui::SameLine();
			if (ImGui::SmallButton((selected_gameobject->GetLayer() + "##layers").c_str())) {
				ImGui::OpenPopup("Layers");
			}
			if (ImGui::BeginPopup("Layers")) {
				//This should be the layers! now is a test
				for (int i = 0; i < 5; i++) {
					std::string name = "layer" + std::to_string(i);
					if (ImGui::MenuItem(name.c_str())) {
						selected_gameobject->SetLayer(name);
					}
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
						CONSOLE_LOG("GameObject can't have more than 1 Mesh Renderer!");
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

		if (ImGui::DragFloat3("Position", (float*)&position, 0.25f)) {
			transform->SetPosition(position);
		}
		if (ImGui::DragFloat3("Rotation", (float*)&rotation, 0.25f, -360, 360)) {
			transform->SetRotation(rotation);
		}
		if (ImGui::DragFloat3("Scale", (float*)&scale, 0.25f)) {
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
	}
}
