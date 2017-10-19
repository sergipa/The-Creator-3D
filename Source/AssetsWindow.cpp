#include "AssetsWindow.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "Resource.h"
#include "ModuleResources.h"
#include "ModuleImport.h"
#include "tinyfiledialogs.h"
#include "Texture.h"

AssetsWindow::AssetsWindow()
{
	active = true;
	window_name = "Assets";

	node = 0;
	show_new_folder_window = false;
	file_options_open = true;
	texture_icon = nullptr;
	mesh_icon = App->import->LoadEngineImages(EDITOR_IMAGES_FOLDER"mesh_icon.png");
	font_icon = App->import->LoadEngineImages(EDITOR_IMAGES_FOLDER"font_icon.png");
	folder_icon = App->import->LoadEngineImages(EDITOR_IMAGES_FOLDER"folder_icon.png");

	if (!fs::exists(ASSETS_FOLDER)) {
		if (!fs::create_directory(PROJECT_FOLDER"Assets")) {
			CONSOLE_ERROR("Assets folder is not found and can't create new folder");
			return;
		}
	}
	selected_folder = ASSETS_FOLDER;
	FillAssetsLists();
}

AssetsWindow::~AssetsWindow()
{
	RELEASE(texture_icon);
	RELEASE(mesh_icon);
	RELEASE(font_icon);
}

void AssetsWindow::DrawWindow()
{
	if (ImGui::BeginDock(window_name.c_str(), false, false/*, App->IsPlaying()*/, ImGuiWindowFlags_HorizontalScrollbar)) {
		ImGui::Columns(2);
		node = 0;
		ImGui::Spacing();
		DrawChilds(ASSETS_FOLDER);

		if (ImGui::IsMouseClicked(1) && ImGui::IsMouseHoveringWindow()) {
			ImGui::SetNextWindowPos(ImGui::GetMousePos());
			ImGui::OpenPopup("Assets Options");
		}

		if (!selected_folder.empty()) {
			if (ImGui::BeginPopup("Assets Options"))
			{
				if (ImGui::MenuItem("Create Folder")) {
					show_new_folder_window = true;
				}
				if (selected_folder.filename().string() != "Assets") {
					if (ImGui::MenuItem("Delete")) {
						fs::remove_all(selected_folder);
					}
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Import Texture")) {
					char const * lFilterPatterns[4] = { "*.jpg", "*.png", "*.tga", "*.dds" };
					const char* spritePath = tinyfd_openFileDialog("Select Texture...", NULL, 4, lFilterPatterns, NULL, 0);
					if (spritePath != NULL) {
						fs::path oldPath(spritePath);
						fs::path newPath(selected_folder.string() + "\\" + oldPath.filename().string());
						if (!fs::exists(newPath)) {
							if (oldPath != newPath) {
								fs::copy_file(oldPath, newPath);
							}
							else {
								tinyfd_messageBox("Error", "Open file name is NULL", "ok", "error", 1);
							}
						}
						else {
							tinyfd_messageBox("Error", "A file with this name exist in current folder", "ok", "error", 1);
						}
					}
				}
				ImGui::EndPopup();
			}
		}

		if (show_new_folder_window) {
			ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowSize().x / 2, ImGui::GetWindowSize().y / 2));
			ImGui::SetNextWindowPosCenter();
			ImGui::Begin("New Folder Name", &active,
				ImGuiWindowFlags_NoFocusOnAppearing |
				ImGuiWindowFlags_AlwaysAutoResize |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_ShowBorders |
				ImGuiWindowFlags_NoTitleBar);
			ImGui::Spacing();
			ImGui::Text("New Folder Name");
			static char inputText[20];
			ImGui::InputText("", inputText, 20);
			ImGui::Spacing();
			if (ImGui::Button("Confirm")) {
				std::string str(inputText);
				fs::path temp = selected_folder;
				if (fs::create_directory(selected_folder += ("\\" + str))) {
					show_new_folder_window = false;
				}
				else {
					selected_folder = temp;
				}
				strcpy(inputText, "");
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel")) {
				strcpy(inputText, "");
				show_new_folder_window = false;
			}
			ImGui::End();
		}

		ImGui::NextColumn();

		if (ImGui::BeginChild("Files", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar/*, App->IsPlaying()*/)) 
		{
			if (!selected_folder.empty()) 
			{
				for (auto & p : fs::directory_iterator(selected_folder)) 
				{
					if (!fs::is_directory(p)) 
					{
						bool selected = false;
						float font_size = ImGui::GetFontSize();
						switch (ExtensionToResourceType(p.path().extension().string()))
						{
						case Resource::TextureResource:
							texture_icon = App->resources->GetTexture(p.path().stem().string());
							ImGui::Image((ImTextureID)texture_icon->GetID(), { font_size, font_size }, ImVec2(0, 1), ImVec2(1, 0));
							ImGui::SameLine();
							break;
						case Resource::MeshResource:
							ImGui::Image((ImTextureID)mesh_icon->GetID(), { font_size, font_size }, ImVec2(0, 1), ImVec2(1, 0));
							ImGui::SameLine();
							break;
						case Resource::FontResource:
							ImGui::Image((ImTextureID)font_icon->GetID(), { font_size, font_size }, ImVec2(0, 1), ImVec2(1, 0));
							ImGui::SameLine();
							break;
						case Resource::Unknown:
							continue; //if the type is unknown skip and don't draw the file in the panel
							break;
						}
						
						if (p.path() == selected_file_path) {
							if (App->scene->selected_gameobjects.empty()) {
								selected = true;
							}
							else {
								selected_file_path.clear();
							}
						}
						ImGui::Selectable(p.path().filename().string().c_str(), &selected);
						if (ImGui::IsItemHoveredRect()) {
							if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1) && !file_options_open) {
								selected_file_path = p.path();
								App->scene->selected_gameobjects.clear();
								if (ImGui::IsItemClicked(1)) {
									ImGui::SetNextWindowPos(ImGui::GetMousePos());
									ImGui::OpenPopup("File Options");
									file_options_open = true;
								}
							}
						}
					}
				}
			}

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
	for (auto & p : fs::recursive_directory_iterator(ASSETS_FOLDER))
	{
		if (!fs::is_directory(p)) 
		{
			switch (ExtensionToResourceType(p.path().extension().string()))
			{
			case Resource::TextureResource:
				App->import->LoadTexture(p.path().string().c_str(), false);
				break;
			case Resource::MeshResource:
				//App->import->LoadMesh(p.path().string().c_str());
				break;
			}
		}
	}
}

void AssetsWindow::DrawChilds(fs::path path)
{
	std::string path_name;
	if (path == ASSETS_FOLDER) path_name = "Assets"; //The "." in ASSETS_FOLDER causes problems
	else path_name = path.filename().string();
	sprintf_s(node_name, 30, "%s##node_%i", path_name.c_str(), node++);
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

	if (ImGui::TreeNodeExI(node_name, (ImTextureID)folder_icon->GetID(), flag))
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

int AssetsWindow::ExtensionToResourceType(std::string str)
{
	if (str == ".jpg" || str == ".png" || str == ".tga" || str == ".dds") return Resource::TextureResource;
	else if (str == ".fbx") return Resource::MeshResource;
	else if (str == ".wav" || str == ".ogg") return Resource::AudioResource;
	else if (str == ".prefab") return Resource::PrefabResource;
	else if (str == ".animation") return Resource::AnimationResource;
	else if (str == ".particleFX") return Resource::ParticleFXResource;
	else if (str == ".scene") return Resource::SceneResource;
	else if (str == ".ttf") return Resource::FontResource;
	
	return Resource::Unknown;
}