#include "SceneWindow.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"

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

		HandleInput();
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
			int max_msaa_level = App->renderer3D->textureMSAA->GetMaxMSAALevel();
			int curr_msaa_level = App->renderer3D->textureMSAA->GetCurrentMSAALevel();
			bool selected = false;
			for (int i = 0; i <= max_msaa_level; i *= 2)
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
		ImGui::EndMenuBar();
	}

	if (wireframe_mode)
	{
		App->renderer3D->SetWireframeMode();
	}
}

void SceneWindow::HandleInput()
{
	if (ImGui::IsMouseDown(1) || App->input->GetMouseZ() > 0 || App->input->GetMouseZ() < 0)
	{
		if (ImGui::IsMouseHoveringWindow())
		{
			App->camera->can_update = true;
		}
	}
	if (ImGui::IsMouseReleased(1))
	{
		App->camera->can_update = false;
	}
}
