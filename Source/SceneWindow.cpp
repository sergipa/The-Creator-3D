#include "SceneWindow.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ComponentCamera.h"
#include "RenderTexture.h"
#include "ModuleScene.h"

SceneWindow::SceneWindow()
{
	active = true;
	window_name = "Scene";
	scene_width = 0;
	scene_height = 0;
	wireframe_mode = false;
}

SceneWindow::~SceneWindow()
{
}

void SceneWindow::DrawWindow()
{
	if (ImGui::BeginDock(window_name.c_str(), false, false, false, 
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_MenuBar)) {


		ImVec2 size = ImGui::GetContentRegionAvail();
		if (scene_width != size.x && scene_height != size.y) {
			App->renderer3D->OnResize(size.x, size.y);
		}

		DrawMenuBar();

		ImGui::Image((void*)App->renderer3D->textureMSAA->GetTexture(), size, ImVec2(0, 1), ImVec2(1, 0));
		
		if (App->renderer3D->active_camera != nullptr)
		{
			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			//draw_list->AddImage((void*)App->renderer3D->active_camera->GetViewportTexture()->GetTexture(), { 600,400 }, { 900,700 }, ImVec2(0, 1), ImVec2(1, 0));
		}

		is_mouse_hovering_window = ImGui::IsMouseHoveringWindow();
		//Necessary because left-click doesn't give focus to a window
		if (ImGui::IsMouseClicked(0) || ImGui::IsMouseClicked(1))
		{
			if(is_mouse_hovering_window) is_window_focused = true;
			else is_window_focused = false;
		}
	}

	ImGui::EndDock();
}

void SceneWindow::DrawMenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Shading Mode"))
		{
			if (ImGui::MenuItem("Shaded", "", !wireframe_mode))
			{
				wireframe_mode = false;
			}
			if (ImGui::MenuItem("Wireframe", "", wireframe_mode))
			{
				wireframe_mode = true;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Anti Aliasing level"))
		{
			int curr_msaa_level = App->renderer3D->textureMSAA->GetCurrentMSAALevel();
			bool selected = false;
			for (int i = 0; i <= 8; i *= 2) //cap the max MSAA level at x8
			{
				char msaa_level[5];
				sprintf(msaa_level, "x%d", i);
				if (i == curr_msaa_level) selected = true;
				if (ImGui::MenuItem(msaa_level, "", selected))
				{
					App->renderer3D->textureMSAA->ChangeMSAALevel(i);
				}
				if (i == 0) i++;
				selected = false;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Gizmos"))
		{
			bool selected = App->scene->draw_octree;
			if (ImGui::MenuItem("Octree", "", selected))
			{
				App->scene->draw_octree = !selected;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if (wireframe_mode)
	{
		App->renderer3D->SetWireframeMode();
	}
}

bool SceneWindow::IsWindowFocused() const
{
	return is_window_focused;
}

bool SceneWindow::IsMouseHoveringWindow() const
{
	return is_mouse_hovering_window;
}
