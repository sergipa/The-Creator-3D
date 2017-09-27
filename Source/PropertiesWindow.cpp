#include "PropertiesWindow.h"
#include "Application.h"

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
		//GameObject* selected_game_object = nullptr;
		//if (engine->sceneManagerModule->selected_game_objects.size() == 1) {
		//	selected_game_object = engine->sceneManagerModule->selected_game_objects.front();
		//}

		//if (selected_game_object != nullptr) {
		//	bool gameObjectActive = selected_game_object->isActive();
		//	ImGui::SameLine();
		//	if (ImGui::Checkbox("", &gameObjectActive)) {
		//		selected_game_object->SetActive(gameObjectActive);
		//	}
		//	ImGui::SameLine();
		//	ImGui::Text("Name: %s", selected_game_object->name.c_str());
		//	ImGui::Text("Tag:");
		//	ImGui::SameLine();
		//	if (ImGui::SmallButton(selected_game_object->tag.c_str())) {
		//		ImGui::OpenPopup("Tags");
		//	}
		//	if (ImGui::BeginPopup("Tags")) {
		//		for (int i = 0; i < 5; i++) {
		//			string name = "tag" + to_string(i);
		//			if (ImGui::MenuItem(name.c_str())) {
		//				selected_game_object->tag = name;
		//			}
		//		}
		//		ImGui::EndPopup();
		//	}
		//	ImGui::SameLine();
		//	ImGui::Text("Layer:");
		//	ImGui::SameLine();
		//	if (ImGui::SmallButton(selected_game_object->layer.c_str())) {
		//		ImGui::OpenPopup("Layers");
		//	}
		//	if (ImGui::BeginPopup("Layers")) {
		//		for (int i = 0; i < 5; i++) {
		//			string name = "layer" + to_string(i);
		//			if (ImGui::MenuItem(name.c_str())) {
		//				selected_game_object->layer = name;
		//			}
		//		}
		//		ImGui::EndPopup();
		//	}
		//	ImGui::Spacing();
		//	ImGui::Separator();
		//	ImGui::Spacing();

		//	for (list<Component*>::iterator it = selected_game_object->components_list.begin(); it != selected_game_object->components_list.end(); it++) {
		//		DrawComponent((*it));
		//		ImGui::Separator();
		//		ImGui::Spacing();
		//	}

		//	if (ImGui::Button("Add Component")) {
		//		ImGui::OpenPopup("Components");
		//	}
		//	if (ImGui::BeginPopup("Components"))
		//	{
		//		//if (ImGui::MenuItem("RigidBody")) {
		//		//	if (selected_game_object->GetComponent(Component::RigidBody) == nullptr) {
		//		//		selected_game_object->AddComponent(Component::RigidBody);
		//		//	}
		//		//}
		//		if (ImGui::MenuItem("Sprite Renderer")) {
		//			if (selected_game_object->GetComponent(Component::SpriteRenderer) == nullptr) {
		//				selected_game_object->AddComponent(Component::SpriteRenderer);
		//			}
		//		}
		//		if (ImGui::MenuItem("Animation")) {
		//			if (selected_game_object->GetComponent(Component::Animaton) == nullptr) {
		//				selected_game_object->AddComponent(Component::Animaton);
		//			}
		//		}
		//		if (ImGui::MenuItem("Audio")) {
		//			if (selected_game_object->GetComponent(Component::AudioSource) == nullptr) {
		//				selected_game_object->AddComponent(Component::AudioSource);
		//			}
		//		}
		//		if (ImGui::MenuItem("Particle System")) {
		//			if (selected_game_object->GetComponent(Component::ParticleSystem) == nullptr) {
		//				selected_game_object->AddComponent(Component::ParticleSystem);
		//			}
		//		}
		//		if (ImGui::BeginMenu("Scripts")) {
		//			if (App->resourcesModule->GetGameScriptsList().empty()) {
		//				ImGui::MenuItem("No scripts");
		//			}
		//			else {
		//				for (int i = 0; i < engine->resourcesModule->GetGameScriptsList().size(); i++) {
		//					if (ImGui::MenuItem(engine->resourcesModule->GetGameScriptsList()[i]->GetName().c_str())) {
		//						ComponentScript* script = (ComponentScript*)selected_game_object->AddComponent(Component::Script);
		//						script->SetScriptName(engine->resourcesModule->GetGameScriptsList()[i]->GetName().c_str());
		//						script->SetScriptPath(engine->resourcesModule->GetGameScriptsList()[i]->GetScript());
		//					}
		//				}
		//			}
		//			ImGui::EndMenu();
		//		}

		//		ImGui::EndPopup();
		//	}
		//}
	}

	ImGui::EndDock();
}
