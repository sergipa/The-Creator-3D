#include "HierarchyWindow.h"
#include "Application.h"

HierarchyWindow::HierarchyWindow()
{
	active = true;
	window_name = "Hierarchy";
}

HierarchyWindow::~HierarchyWindow()
{
}

void HierarchyWindow::DrawWindow()
{
	if (ImGui::BeginDock(window_name.c_str(), false, false, false, ImGuiWindowFlags_HorizontalScrollbar)) {
		if (ImGui::IsMouseClicked(1) && ImGui::IsMouseHoveringWindow()/* && !App->IsPlaying()*/) {
			ImGui::SetNextWindowPos(ImGui::GetMousePos());
			ImGui::OpenPopup("GameObject Options");
		}

		if (ImGui::BeginPopup("GameObject Options"))
		{
			//if (!App->scene_manager_module->selectedGameObjects.empty()) {
			//	if (ImGui::MenuItem("Duplicate")) {
			//		for (list<GameObject*>::iterator it = App->scene_manager_module->selectedGameObjects.begin(); it != App->scene_manager_module->selectedGameObjects.end(); it++) {
			//			if ((*it)->parent != nullptr) {
			//				if (find(App->scene_manager_module->selectedGameObjects.begin(), App->scene_manager_module->selectedGameObjects.end(), (*it)->parent) != App->scene_manager_module->selectedGameObjects.end()) {
			//					continue; //If parent will be duplicated skip this because parent will take care of childs;
			//				}
			//			}
			//			App->scene_manager_module->DuplicateGameObject(*it);
			//		}
			//	}
			//	if (ImGui::MenuItem("Delete")) {
			//		for (list<GameObject*>::iterator it = App->scene_manager_module->selectedGameObjects.begin(); it != App->scene_manager_module->selectedGameObjects.end(); it++) {
			//			(*it)->Destroy();
			//		}
			//	}
			//	if (App->scene_manager_module->selectedGameObjects.size() == 1) {
			//		if (ImGui::MenuItem("Rename")) {
			//			show_rename_window = true;
			//			gameobject_to_rename = App->scene_manager_module->selectedGameObjects.front();
			//		}
			//		if (ImGui::MenuItem("Create Child")) {
			//			GameObject* parent = nullptr;
			//			parent = App->scene_manager_module->selectedGameObjects.front();
			//			App->scene_manager_module->CreateGameObject(parent);
			//			open_game_object_node = parent;
			//		}
			//	}
			//	ImGui::Separator();
			//}
			if (ImGui::MenuItem("Create Game Object")) {
				//App->scene_manager_module->CreateGameObject(nullptr);
			}

			ImGui::EndPopup();
		}

		if (show_rename_window) {
			ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowSize().x + 20, rename_window_y));
			bool windowActive = true;
			ImGui::Begin("Rename Game Object", &windowActive,
				ImGuiWindowFlags_NoFocusOnAppearing |
				ImGuiWindowFlags_AlwaysAutoResize |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_ShowBorders |
				ImGuiWindowFlags_NoTitleBar);

			ImGui::Text("Enter new name");
			static char inputText[20];
			ImGui::InputText("", inputText, 20);
			if (show_rename_error) {
				ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "Name cannot be blank");
			}
			if (ImGui::Button("Confirm")) {
				bool isBlankString = true;
				for (int i = 0; inputText[i] != '\0'; i++) {
					if (inputText[i] != ' ') {
						isBlankString = false;
						break;
					}
				}
				if (!isBlankString) {
					//App->scene_manager_module->selectedGameObjects.front()->Rename(inputText);
					//App->scene_manager_module->RenameDuplicatedGameObject(engine->sceneManagerModule->selectedGameObjects.front());
					show_rename_error = false;
					show_rename_window = false;
				}
				else {
					show_rename_error = true;
				}
				inputText[0] = '\0';
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel")) {
				show_rename_window = false;
				inputText[0] = '\0';
			}
			ImGui::End();
		}

		//for (list<GameObject*>::iterator it = App->scene_manager_module->sceneRootObjects.begin(); it != App->scene_manager_module->sceneRootObjects.end(); it++) {
		//	DrawChilds(*it);
		//}
	}

	ImGui::EndDock();
}
