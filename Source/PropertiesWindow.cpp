#include "PropertiesWindow.h"
#include "Application.h"
#include "GameObject.h"
#include "glmath.h"

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
		vec3 position;
		vec3 rotation;
		vec3 scale;
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
			ImGui::Text("Texture Path: %s", mesh_renderer->GetTexture()->GetPath().c_str());
			if (ImGui::IsItemHoveredRect() && ImGui::CalcTextSize(("Texture Path: " + mesh_renderer->GetTexture()->GetPath()).c_str()).x > ImGui::GetContentRegionAvailWidth()) {
				ImGui::BeginTooltip();
				ImGui::Text("%s", mesh_renderer->GetTexture()->GetPath().c_str());
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
