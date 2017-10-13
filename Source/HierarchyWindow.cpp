#include "HierarchyWindow.h"
#include "Application.h"
#include "GameObject.h"
#include "ModuleInput.h"

HierarchyWindow::HierarchyWindow()
{
	active = true;
	window_name = "Hierarchy";
	gameobject_to_rename = nullptr;
	show_rename_window = false;
	show_rename_error = false;
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
			if (!App->scene->selected_gameobjects.empty()) {
				if (ImGui::MenuItem("Duplicate")) {
					for (std::list<GameObject*>::iterator it = App->scene->selected_gameobjects.begin(); it != App->scene->selected_gameobjects.end(); it++) {
						if ((*it)->GetParent() != nullptr) {
							if (std::find(App->scene->selected_gameobjects.begin(), App->scene->selected_gameobjects.end(), (*it)->GetParent()) != App->scene->selected_gameobjects.end()) {
								continue; //If parent will be duplicated skip this because parent will take care of childs;
							}
						}
						App->scene->DuplicateGameObject(*it);
					}
				}
				if (ImGui::MenuItem("Delete")) {
					for (std::list<GameObject*>::iterator it = App->scene->selected_gameobjects.begin(); it != App->scene->selected_gameobjects.end(); it++) {
						(*it)->Destroy();
					}
				}
				if (App->scene->selected_gameobjects.size() == 1) {
					if (ImGui::MenuItem("Rename")) {
						show_rename_window = true;
						gameobject_to_rename = App->scene->selected_gameobjects.front();
					}
					if (ImGui::MenuItem("Create Child")) {
						GameObject* parent = nullptr;
						parent = App->scene->selected_gameobjects.front();
						App->scene->CreateGameObject(parent);
						//open_game_object_node = parent;
					}
				}
				ImGui::Separator();
			}
			if (ImGui::MenuItem("Create Game Object")) {
				App->scene->CreateGameObject(nullptr);
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
					App->scene->selected_gameobjects.front()->SetName(inputText);
					App->scene->RenameDuplicatedGameObject(App->scene->selected_gameobjects.front());
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

		for (std::list<GameObject*>::iterator it = App->scene->root_gameobjects.begin(); it != App->scene->root_gameobjects.end(); it++) {
			DrawSceneGameObjects(*it);
		}
	}

	ImGui::EndDock();
}

void HierarchyWindow::DrawSceneGameObjects(GameObject * gameObject)
{
	uint flag = 0;

	if (gameObject->childs.empty()) {
		flag |= ImGuiTreeNodeFlags_Leaf;
	}

	for (std::list<GameObject*>::iterator it = App->scene->selected_gameobjects.begin(); it != App->scene->selected_gameobjects.end(); it++) {
		if (gameObject == *it) {
			flag |= ImGuiTreeNodeFlags_Selected;
			break;
		}
	}

	flag |= ImGuiTreeNodeFlags_OpenOnArrow;

	/*if (openGameObjectNode == gameObject) {
		ImGui::SetNextTreeNodeOpen(true);
		openGameObjectNode = nullptr;
	}*/

	if (ImGui::TreeNodeEx(gameObject->GetName().c_str(), flag))
	{
		IsMouseOver(gameObject);
		for (std::list<GameObject*>::iterator it = gameObject->childs.begin(); it != gameObject->childs.end(); ++it) {
			DrawSceneGameObjects(*it);
		}
		ImGui::TreePop();
	}
	else 
	{
		IsMouseOver(gameObject);
	}
}

void HierarchyWindow::IsMouseOver(GameObject * gameObject)
{
	if (ImGui::IsMouseClicked(0) || ImGui::IsMouseClicked(1)) {
		if (ImGui::IsItemHoveredRect())
		{
			std::list<GameObject*>::iterator it;

			if (!App->scene->selected_gameobjects.empty()) {
				it = find(App->scene->selected_gameobjects.begin(), App->scene->selected_gameobjects.end(), gameObject);
				if (it == App->scene->selected_gameobjects.end()) {
					if (App->input->GetKey(SDL_SCANCODE_LCTRL) != KEY_REPEAT) App->scene->selected_gameobjects.clear();
					App->scene->selected_gameobjects.push_back(gameObject);
				}
			}
			else {
				App->scene->selected_gameobjects.push_back(gameObject);
			}
		}
		else {
			if (ImGui::IsMouseHoveringWindow() && !ImGui::IsMouseClicked(1)) {
				if (App->input->GetKey(SDL_SCANCODE_LCTRL) != KEY_REPEAT && !App->scene->selected_gameobjects.empty() &&
					!show_rename_error) {
					App->scene->selected_gameobjects.remove(gameObject);
				}
			}
		}
	}

	if (ImGui::IsMouseDoubleClicked(0)/* && !App->IsPlaying()*/)
	{
		if (ImGui::IsItemHoveredRect())
		{
			if (!show_rename_error) {
				show_rename_error = true;
				rename_window_y = ImGui::GetMousePos().y;
			}
		}
	}
}
