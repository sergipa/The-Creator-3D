#include "AssetsWindow.h"
#include "Application.h"
//#include "tinyfiledialogs.h"
#include "ModuleEditor.h"

AssetsWindow::AssetsWindow()
{
	active = true;
	window_name = "Assets";

	/*assets_path = ASSETS_FOLDER;
	if (!fs::exists(assets_path)) {
		if (fs::create_directory(ASSETS_FOLDER)) {
			assets_path = ASSETS_FOLDER;
		}
		else {
			LOG_ERROR("Assets folder is not found and can't create new folder");
			return;
		}
	}*/
	selected_folder = assets_path;
	FillAssetsLists();
}

AssetsWindow::~AssetsWindow()
{
	/*delete folder_image;
	delete sound_image;
	delete texture_image;
	delete lua_script_image;
	delete tool_tip_texture;*/
}

void AssetsWindow::DrawWindow()
{
	if (ImGui::BeginDock(window_name.c_str(), false, false/*, App->IsPlaying()*/, ImGuiWindowFlags_HorizontalScrollbar)) {
		ImGui::Columns(2);
		//node = 0;
		//ImGui::Spacing();
		//DrawChilds(assets_path);

		//if (ImGui::IsMouseClicked(1) && ImGui::IsMouseHoveringWindow()) {
		//	ImGui::SetNextWindowPos(ImGui::GetMousePos());
		//	ImGui::OpenPopup("Assets Options");
		//}

		//if (!selected_folder.empty()) {
		//	if (ImGui::BeginPopup("Assets Options"))
		//	{
		//		if (ImGui::MenuItem("Create Folder")) {
		//			show_new_folder_window = true;
		//		}
		//		if (selected_folder.filename().string() != "Assets") {
		//			if (ImGui::MenuItem("Delete")) {
		//				fs::remove_all(selected_folder);
		//			}
		//		}
		//		/*if (ImGui::MenuItem("Create C++ Script")) {
		//		showNewScriptWindow = true;
		//		scriptType = hScript;
		//		}*/
		//		/*if (ImGui::MenuItem("Create Lua Script")) {
		//			show_new_folder_window = true;
		//			script_type = luaScript;
		//		}*/
		//		ImGui::Separator();
		//		if (ImGui::MenuItem("Import Sprite")) {
		//			char const * lFilterPatterns[2] = { "*.jpg", "*.png" };
		//			const char* spritePath = tinyfd_openFileDialog("Select Sprite...", NULL, 2, lFilterPatterns, NULL, 0);
		//			if (spritePath != NULL) {
		//				fs::path oldPath(spritePath);
		//				fs::path newPath(selected_folder.string() + "\\" + oldPath.filename().string());
		//				if (!fs::exists(newPath)) {
		//					if (oldPath != newPath) {
		//						fs::copy_file(oldPath, newPath);
		//					}
		//					else {
		//						tinyfd_messageBox("Error", "Open file name is NULL", "ok", "error", 1);
		//					}
		//				}
		//				else {
		//					tinyfd_messageBox("Error", "A file with this name exist in current folder", "ok", "error", 1);
		//				}
		//			}
		//		}
		//		ImGui::EndPopup();
		//	}
		//}

		//if (show_new_folder_window && !show_new_script_window) {
		//	ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowSize().x / 2, ImGui::GetWindowSize().y / 2));
		//	ImGui::SetNextWindowPosCenter();
		//	ImGui::Begin("New Folder Name", &active,
		//		ImGuiWindowFlags_NoFocusOnAppearing |
		//		ImGuiWindowFlags_AlwaysAutoResize |
		//		ImGuiWindowFlags_NoCollapse |
		//		ImGuiWindowFlags_ShowBorders |
		//		ImGuiWindowFlags_NoTitleBar);
		//	ImGui::Spacing();
		//	ImGui::Text("New Folder Name");
		//	static char inputText[20];
		//	ImGui::InputText("", inputText, 20);
		//	ImGui::Spacing();
		//	if (ImGui::Button("Confirm")) {
		//		std::string str(inputText);
		//		fs::path temp = selected_folder;
		//		if (fs::create_directory(selected_folder += ("\\" + str))) {
		//			show_new_folder_window = false;
		//		}
		//		else {
		//			selected_folder = temp;
		//		}
		//		strcpy(inputText, "");
		//	}
		//	ImGui::SameLine();
		//	if (ImGui::Button("Cancel")) {
		//		strcpy(inputText, "");
		//		show_new_folder_window = false;
		//	}
		//	ImGui::End();
		//}

		///*if (show_new_folder_window && !show_new_folder_window) {
		//	ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowSize().x / 2, ImGui::GetWindowSize().y / 2));
		//	ImGui::SetNextWindowPosCenter();
		//	ImGui::Begin("New Script Name", &active,
		//		ImGuiWindowFlags_NoFocusOnAppearing |
		//		ImGuiWindowFlags_AlwaysAutoResize |
		//		ImGuiWindowFlags_NoCollapse |
		//		ImGuiWindowFlags_ShowBorders |
		//		ImGuiWindowFlags_NoTitleBar);
		//	ImGui::Spacing();
		//	ImGui::Text("New Script Name");
		//	static char inputText[30];
		//	ImGui::InputText("", inputText, 30);
		//	ImGui::Spacing();
		//	if (ImGui::Button("Confirm")) {
		//		std::string str(inputText);
		//		if (!str.empty()) {
		//			str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
		//			CreateScript(hScript, inputText);
		//			strcpy(inputText, "");
		//			show_new_script_window = false;
		//		}
		//	}
		//	ImGui::SameLine();
		//	if (ImGui::Button("Cancel")) {
		//		strcpy(inputText, "");
		//		show_new_script_window = false;
		//	}
		//	ImGui::End();
		//}*/

		ImGui::NextColumn();

		//Need to optimize this!!
		if (ImGui::BeginChild("Files", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar/*, App->IsPlaying()*/)) {
			//if (!selected_folder.empty()) {
			//	for (auto & p : fs::directory_iterator(selected_folder)) {
			//		if (!fs::is_directory(p)) {
			//			bool selected = false;
			//			switch (String2int(p.path().extension().string())) {
			//			case 0: //wav file
			//				ImGui::Image(sound_image, { 16,16 });
			//				ImGui::SameLine();
			//				if (p.path() == selected_file_path) {
			//					if (App->scene_manager_module->selected_gameobjects.empty()) {
			//						selected = true;
			//					}
			//					else {
			//						selected_file_path.clear();
			//					}
			//				}
			//				ImGui::Selectable(p.path().filename().string().c_str(), &selected, 0, { 0,0 }, false);
			//				if (ImGui::IsItemHoveredRect()) {
			//					if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1) && !file_options_open) {
			//						selected_file_path = p.path();
			//						App->scene_manager_module->selected_gameobjects.clear();
			//						if (ImGui::IsItemClicked(1)) {
			//							ImGui::SetNextWindowPos(ImGui::GetMousePos());
			//							ImGui::OpenPopup("File Options");
			//							file_options_open = true;
			//						}
			//					}
			//				}
			//				break;
			//			case 1: // ogg file
			//				//ImGui::Image(*soundImage, { 16,16 }, sf::Color::White, sf::Color::Transparent);
			//				//ImGui::SameLine();
			//				if (p.path() == selected_file_path) {
			//					if (App->scene_manager_module->selected_gameobjects.empty()) {
			//						selected = true;
			//					}
			//					else {
			//						selected_file_path.clear();
			//					}
			//				}
			//				ImGui::Selectable(p.path().filename().string().c_str(), &selected, 0, { 0,0 }, false);
			//				if (ImGui::IsItemHoveredRect()) {
			//					if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1) && !file_options_open) {
			//						selected_file_path = p.path();
			//						App->scene_manager_module->selected_gameobjects.clear();
			//						if (ImGui::IsItemClicked(1)) {
			//							ImGui::SetNextWindowPos(ImGui::GetMousePos());
			//							ImGui::OpenPopup("File Options");
			//							file_options_open = true;
			//						}
			//					}
			//				}
			//				break;
			//			case 2: //jpg/png file
			//				ImGui::Image(texture_image, { 16,16 });
			//				ImGui::SameLine();
			//				if (p.path() == selected_file_path) {
			//					if (App->scene_manager_module->selected_gameobjects.empty()) {
			//						selected = true;
			//					}
			//					else {
			//						selected_file_path.clear();
			//					}
			//				}
			//				ImGui::Selectable(p.path().filename().string().c_str(), &selected, 0, { 0,0 }, false);
			//				if (ImGui::IsItemHoveredRect()) {
			//					if (!file_options_open && !App->editor_module->drag_data.has_data) {
			//						ImGui::BeginTooltip();
			//						/*if (toolTiptexture.loadFromFile(p.path().string())) {
			//							toolTiptexture.setSmooth(true);
			//							ImGui::Image(toolTiptexture, { 50,50 }, sf::Color::White, sf::Color::Transparent);
			//						}*/
			//						ImGui::EndTooltip();
			//					}
			//					if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1) && !file_options_open) {
			//						selected_file_path = p.path();
			//						App->scene_manager_module->selected_gameobjects.clear();
			//						if (ImGui::IsItemClicked(1)) {
			//							ImGui::SetNextWindowPos(ImGui::GetMousePos());
			//							ImGui::OpenPopup("File Options");
			//							file_options_open = true;
			//						}
			//						else {
			//							if (!App->editor_module->drag_data.has_data) {
			//								App->editor_module->drag_data.from_panel = "Files";
			//								App->editor_module->drag_data.path = p.path().string();
			//								App->editor_module->drag_data.extension = p.path().extension().string();
			//								App->editor_module->drag_data.name = p.path().filename().replace_extension().string();
			//								App->editor_module->drag_data.has_data = true;
			//								App->editor_module->drag_data.drag_sprite = tool_tip_texture;
			//							}
			//						}
			//					}
			//				}
			//				break;
			//			case 3: //LUA Script
			//				/*ImGui::Image(*luaScriptImage, { 16,16 }, sf::Color::White, sf::Color::Transparent);
			//				ImGui::SameLine();
			//				if (p.path() == selected_file_path) {
			//					if (App->scene_manager_module->selected_gameobjects.empty()) {
			//						selected = true;
			//					}
			//					else {
			//						selected_file_path.clear();
			//					}
			//				}
			//				ImGui::Selectable(p.path().filename().string().c_str(), &selected, 0, { 0,0 }, false);
			//				if (ImGui::IsItemHoveredRect()) {
			//					if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1) && !file_options_open) {
			//						selected_file_path = p.path();
			//						App->scene_manager_module->selected_gameobjects.clear();
			//						if (ImGui::IsItemClicked(1)) {
			//							ImGui::SetNextWindowPos(ImGui::GetMousePos());
			//							ImGui::OpenPopup("File Options");
			//							file_options_open = true;
			//						}
			//					}
			//				}*/
			//				break;
			//			case 4: //animation file
			//				//ImGui::Image(*soundImage, { 16,16 }, sf::Color::White, sf::Color::Transparent);
			//				//ImGui::SameLine();
			//				if (p.path() == selected_file_path) {
			//					if (App->scene_manager_module->selected_gameobjects.empty()) {
			//						selected = true;
			//					}
			//					else {
			//						selected_file_path.clear();
			//					}
			//				}
			//				ImGui::Selectable(p.path().filename().string().c_str(), &selected, 0, { 0,0 }, false);
			//				if (ImGui::IsItemHoveredRect()) {
			//					if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1) && !file_options_open) {
			//						selected_file_path = p.path();
			//						App->scene_manager_module->selected_gameobjects.clear();
			//						if (ImGui::IsItemClicked(1)) {
			//							ImGui::SetNextWindowPos(ImGui::GetMousePos());
			//							ImGui::OpenPopup("File Options");
			//							file_options_open = true;
			//						}
			//					}
			//				}
			//				break;
			//			case 5: //particle fx
			//				//ImGui::Image(*soundImage, { 16,16 }, sf::Color::White, sf::Color::Transparent);
			//				//ImGui::SameLine();
			//				if (p.path() == selected_file_path) {
			//					if (App->scene_manager_module->selected_gameobjects.empty()) {
			//						selected = true;
			//					}
			//					else {
			//						selected_file_path.clear();
			//					}
			//				}
			//				ImGui::Selectable(p.path().filename().string().c_str(), &selected, 0, { 0,0 }, false);
			//				if (ImGui::IsItemHoveredRect()) {
			//					if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1) && !file_options_open) {
			//						selected_file_path = p.path();
			//						App->scene_manager_module->selected_gameobjects.clear();
			//						if (ImGui::IsItemClicked(1)) {
			//							ImGui::SetNextWindowPos(ImGui::GetMousePos());
			//							ImGui::OpenPopup("File Options");
			//							file_options_open = true;
			//						}
			//					}
			//				}
			//				break;
			//			case 6: //prefab file
			//				if (p.path() == selected_file_path) {
			//					if (App->scene_manager_module->selected_gameobjects.empty()) {
			//						selected = true;
			//					}
			//					else {
			//						selected_file_path.clear();
			//					}
			//				}
			//				ImGui::Selectable(p.path().filename().string().c_str(), &selected, 0, { 0,0 }, false);
			//				if (ImGui::IsItemHoveredRect()) {
			//					if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1) && !file_options_open) {
			//						selected_file_path = p.path();
			//						App->scene_manager_module->selected_gameobjects.clear();
			//						if (ImGui::IsItemClicked(1)) {
			//							ImGui::SetNextWindowPos(ImGui::GetMousePos());
			//							ImGui::OpenPopup("File Options");
			//							file_options_open = true;
			//						}
			//						else {
			//							if (!App->editor_module->drag_data.has_data) {
			//								App->editor_module->drag_data.from_panel = "Files";
			//								App->editor_module->drag_data.path = p.path().string();
			//								App->editor_module->drag_data.extension = p.path().extension().string();
			//								App->editor_module->drag_data.name = p.path().filename().replace_extension().string();
			//								App->editor_module->drag_data.has_data = true;
			//							}
			//						}
			//					}
			//				}
			//				break;
			//			default:
			//				if (p.path() == selected_file_path) selected = true;
			//				ImGui::Selectable(p.path().filename().string().c_str(), &selected, 0, { 0,0 }, false);
			//				if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1) && !file_options_open) {
			//					selected_file_path = p.path();
			//					App->scene_manager_module->selected_gameobjects.clear();
			//					if (ImGui::IsItemClicked(1)) {
			//						ImGui::SetNextWindowPos(ImGui::GetMousePos());
			//						ImGui::OpenPopup("File Options");
			//						file_options_open = true;
			//					}
			//				}
			//				break;
			//			}
			//		}
			//	}
			//	if (ImGui::IsMouseHoveringWindow()) {
			//		if (ImGui::IsMouseReleased(0) && App->editor_module->drag_data.has_data) {
			//			if (App->editor_module->drag_data.from_panel == "Hierarchy") {
			//				Data data;
			//				App->editor_module->dragging_gameobject->Save(data);
			//				data.AddInt("GameObjectsCount", App->scene_manager_module->saving_index);
			//				data.SaveAsBinary(selected_folder.string() + "\\" + App->editor_module->dragging_gameobject->name + ".prefab");
			//				App->scene_manager_module->saving_index = 0;
			//				App->editor_module->dragging_gameobject = nullptr;
			//			}
			//			App->editor_module->drag_data.clearData();
			//		}
			//	}
			//}

			if (ImGui::BeginPopup("File Options"))
			{
				if (ImGui::MenuItem("Rename")) {
					file_options_open = false;
				}
				if (ImGui::MenuItem("Delete")) {
					fs::remove(selected_file_path);
					file_options_open = false;
				}

				ImGui::EndPopup();
			}
			else {
				file_options_open = false;
			}
		}
		ImGui::EndChild();
	}
	ImGui::EndDock();
}

void AssetsWindow::FillAssetsLists()
{
	for (auto & p : fs::recursive_directory_iterator(assets_path)) {
		if (!fs::is_directory(p)) {
			/*if (p.path().extension().string() == ".h" || p.path().extension().string() == ".cs" || p.path().extension().string() == ".lua" || p.path().extension().string() == ".js") {
				ResourceScript* script = new ResourceScript(p.path().string(), p.path().filename().replace_extension("").string());
				App->resources_module->AddResourceScript(script);
			}
			else if (p.path().extension().string() == ".png" || p.path().extension().string() == ".jpg") {
				ResourceSprite* sprite = new ResourceSprite(p.path().string(), p.path().filename().replace_extension("").string());
				App->resources_module->AddResourceSprite(sprite);
			}
			else if (p.path().extension().string() == ".animation") {
				ResourceAnimation* animation = new ResourceAnimation(p.path().string(), p.path().filename().replace_extension("").string());
				App->resources_module->AddResourceAnimation(animation);
			}
			else if (p.path().extension().string() == ".wav") {
				ResourceSound* sound = new ResourceSound(p.path().string(), p.path().filename().replace_extension("").string());
				App->resources_module->AddResourceSound(sound);
			}
			else if (p.path().extension().string() == ".ogg") {
				ResourceMusic* music = new ResourceMusic(p.path().string(), p.path().filename().replace_extension("").string());
				App->resources_module->AddResourceMusic(music);
			}
			else if (p.path().extension().string() == ".particleFX") {
				ResourceParticleEffect* particleFX = new ResourceParticleEffect(p.path().string(), p.path().filename().replace_extension("").string());
				App->resources_module->AddResourceParticleFX(particleFX);
			}*/
		}
	}
}

void AssetsWindow::DrawChilds(fs::path path)
{
	sprintf_s(node_name, 30, "%s##node_%i", path.filename().string().c_str(), node++);
	uint flag = 0;

	if (fs::is_empty(path)) {
		flag |= ImGuiTreeNodeFlags_Leaf;
	}
	else {
		for (auto & p : fs::directory_iterator(path)) {
			if (fs::is_directory(p)) {
				flag = 0;
				break; //if fodler contains another folder, it can't be a leaf.
			}
			flag |= ImGuiTreeNodeFlags_Leaf;
		}
	}

	flag |= ImGuiTreeNodeFlags_OpenOnArrow;

	if (selected_folder == path && !show_new_folder_window) {
		flag |= ImGuiTreeNodeFlags_Selected;
	}

	if (ImGui::TreeNodeEx(node_name, flag))
	{
		if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1)) {
			selected_folder = path;
		}
		for (auto & p : fs::directory_iterator(path)) {
			if (fs::is_directory(p)) {
				DrawChilds(p);
			}
		}
		ImGui::TreePop();
	}
	else {
		if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1)) {
			selected_folder = path;
		}
	}
}

int AssetsWindow::String2int(std::string str)
{
	if (str == ".wav") return 0;
	else if (str == ".ogg") return 1;
	else if (str == ".jpg" || str == ".png") return 2;
	else if (str == ".lua") return 3;
	else if (str == ".animation") return 4;
	else if (str == ".particleFX") return 5;
	else if (str == ".prefab") return 6;
	else return -1;
}
