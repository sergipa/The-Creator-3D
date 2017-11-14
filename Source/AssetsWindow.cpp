#include "AssetsWindow.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "Resource.h"
#include "ModuleResources.h"
#include "ModuleTextureImporter.h"
#include "tinyfiledialogs.h"
#include "Texture.h"
#include "ModuleFileSystem.h"
#include "ModuleScene.h"
#include "Data.h"

AssetsWindow::AssetsWindow()
{
	active = true;
	window_name = "Assets";

	node = 0;
	show_new_folder_window = false;
	file_options_open = true;
	texture_icon = nullptr;
	show_delete_window = false;
	mesh_icon = App->texture_importer->LoadTextureFromLibrary(EDITOR_IMAGES_FOLDER"mesh_icon.png");
	font_icon = App->texture_importer->LoadTextureFromLibrary(EDITOR_IMAGES_FOLDER"font_icon.png");
	folder_icon = App->texture_importer->LoadTextureFromLibrary(EDITOR_IMAGES_FOLDER"folder_icon.png");

	if (!App->file_system->DirectoryExist(ASSETS_FOLDER_PATH)) {
		if (!App->file_system->Create_Directory(PROJECT_FOLDER"Assets")) {
			CONSOLE_ERROR("Assets folder is not found and can't create new folder");
			return;
		}
	}
	assets_folder_path = App->file_system->StringToPathFormat(ASSETS_FOLDER_PATH);
	selected_folder = assets_folder_path;
}

AssetsWindow::~AssetsWindow()
{
	RELEASE(mesh_icon);
	RELEASE(font_icon);
	RELEASE(folder_icon)
}

void AssetsWindow::DrawWindow()
{
	if (ImGui::BeginDock(window_name.c_str(), false, false/*, App->IsPlaying()*/, ImGuiWindowFlags_HorizontalScrollbar)) {
		ImGui::Columns(2);
		node = 0;
		ImGui::Spacing();
		DrawChilds(assets_folder_path);

		if (ImGui::IsMouseClicked(1) && ImGui::IsMouseHoveringWindow()) {
			ImGui::SetNextWindowPos(ImGui::GetMousePos());
			ImGui::OpenPopup("Assets Options");
		}

		if (!App->file_system->DirectoryIsEmpty(selected_folder)) {
			if (ImGui::BeginPopup("Assets Options"))
			{
				if (ImGui::MenuItem("Create Folder")) {
					show_new_folder_window = true;
				}
				if (App->file_system->GetDirectoryName(selected_folder) != "Assets") {
					if (ImGui::MenuItem("Delete")) {
						show_delete_window = true;
						delete_path = selected_folder;
					}
				}
				ImGui::Separator();
				/*if (ImGui::MenuItem("Import Texture")) {
					char const * lFilterPatterns[4] = { "*.jpg", "*.png", "*.tga", "*.dds" };
					const char* texture_path = tinyfd_openFileDialog("Select Texture...", NULL, 4, lFilterPatterns, NULL, 0);
					if (texture_path != NULL) {
						std::string oldPath(texture_path);
						std::string newPath(selected_folder + "\\" + App->file_system->GetDirectoryName(oldPath));
						if (!App->file_system->DirectoryExist(newPath)) {
							if (oldPath != newPath) {
								App->file_system->Copy_File(oldPath, newPath);
							}
							else {
								tinyfd_messageBox("Error", "Open file name is NULL", "ok", "error", 1);
							}
						}
						else {
							tinyfd_messageBox("Error", "A file with this name exist in the current folder", "ok", "error", 1);
						}
					}
				}*/
				ImGui::EndPopup();
			}
		}

		if (show_new_folder_window) {
			CreateDirectortWindow();
		}

		if (show_delete_window)
		{
			DeleteWindow(delete_path);
		}

		ImGui::NextColumn();

		if (ImGui::BeginChild("Files", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar/*, App->IsPlaying()*/)) 
		{
			if (!selected_folder.empty()) 
			{
				std::vector<std::string> files = App->file_system->GetFilesInDirectory(selected_folder);
				for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); it++)
				{
					bool selected = false;
					float font_size = ImGui::GetFontSize();
					std::string file_extension = App->file_system->GetFileExtension(*it);
					std::string file_name = App->file_system->GetFileNameWithoutExtension(*it);
					Resource::ResourceType type = (Resource::ResourceType)App->resources->AssetExtensionToResourceType(file_extension);
					switch (type)
					{
					case Resource::TextureResource:
						texture_icon = App->resources->GetTexture(file_name);
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

					if (*it == selected_file_path) {
						if (App->scene->selected_gameobjects.empty()) {
							selected = true;
						}
						else {
							selected_file_path.clear();
						}
					}
					ImGui::Selectable((file_name + file_extension).c_str(), &selected);
					if (ImGui::IsItemHoveredRect()) {
						if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1) && !file_options_open) {
							selected_file_path = *it;
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

			if (ImGui::BeginPopup("File Options"))
			{
				if (ImGui::MenuItem("Rename")) {
					file_options_open = false;
				}
				if (ImGui::MenuItem("Delete")) {
					delete_path = selected_file_path;
					show_delete_window = true;
					file_options_open = false;
				}

				std::string extension = App->file_system->GetFileExtension(selected_file_path);
				if (extension == ".prefab" || extension == ".fbx" || extension == ".FBX")
				{
					if (ImGui::MenuItem("Load to scene")) {
						std::string file_name = App->file_system->GetFileNameWithoutExtension(selected_file_path);
						Prefab* prefab = App->resources->GetPrefab(file_name);
						App->scene->LoadPrefab(prefab);
					}
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

void AssetsWindow::DrawChilds(std::string path)
{
	std::string path_name;
	path_name = App->file_system->GetDirectoryName(path);
	sprintf_s(node_name, 150, "%s##node_%i", path_name.c_str(), node++);
	uint flag = 0;

	if (!App->file_system->DirectoryHasSubDirectories(path))
	{
		flag |= ImGuiTreeNodeFlags_Leaf;
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
		std::vector<std::string> sub_directories = App->file_system->GetSubDirectories(path);
		for (std::vector<std::string>::iterator it = sub_directories.begin(); it != sub_directories.end(); it++)
		{
			DrawChilds(*it);
		}
		ImGui::TreePop();
	}
	else {
		if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1)) {
			selected_folder = path;
		}
	}
}

void AssetsWindow::CreateDirectortWindow()
{
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
		std::string temp = selected_folder;
		if (App->file_system->Create_Directory(selected_folder += ("\\" + str))) {
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

void AssetsWindow::DeleteWindow(std::string path)
{
	std::string tittle;
	if (App->file_system->IsDirectory(path))
	{
		tittle = "Delete Diretory";
	}
	else
	{
		tittle = "Delete File";
	}
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowSize().x / 2, ImGui::GetWindowSize().y / 2));
	ImGui::SetNextWindowPosCenter();
	ImGui::Begin(tittle.c_str(), &active,
		ImGuiWindowFlags_NoFocusOnAppearing |
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_ShowBorders |
		ImGuiWindowFlags_NoTitleBar);
	ImGui::Spacing();
	if (App->file_system->IsDirectory(path))
	{
		ImGui::Text("Directory %s will be deleted from disk with all the files inside. Continue?", App->file_system->GetDirectoryName(path).c_str());
	}
	else
	{
		ImGui::Text("File %s will be deleted from disk. Continue?", App->file_system->GetFileName(path).c_str());
	}
	ImGui::Spacing();
	if (ImGui::Button("Confirm")) {
		if (App->file_system->IsDirectory(path))
		{
			std::vector<std::string> files = App->file_system->GetFilesInDirectory(path);
			for (int i = 0; i < files.size(); i++)
			{
				App->file_system->Delete_File(path);
				App->resources->DeleteResource(path);
			}
			App->file_system->DeleteDirectory(path);
		}
		else
		{
			App->file_system->Delete_File(path);
			App->resources->DeleteResource(path);
		}
		show_delete_window = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancel")) {
		show_delete_window = false;
	}
	ImGui::End();
	
}
