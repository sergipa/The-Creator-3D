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
}

SceneWindow::~SceneWindow()
{
}

void SceneWindow::DrawWindow()
{
	if (ImGui::BeginDock(window_name.c_str(), false, false, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
		ImVec2 size = ImGui::GetContentRegionAvail();
		if (scene_width != size.x && scene_height != size.y) {
			App->renderer3D->OnResize(size.x, size.y);
		}
		ImGui::Image((void*)App->renderer3D->texture->GetTexture(), size, ImVec2(0, 1), ImVec2(1, 0));

		if (ImGui::IsMouseDown(1) || ImGui::IsMouseDown(2))
		{
			if (ImGui::IsMouseHoveringWindow())
			{
				App->camera->can_update = true;
			}
		}
		if (ImGui::IsMouseReleased(1) || ImGui::IsMouseReleased(2))
		{
			App->camera->can_update = false;
		}
	}

	ImGui::EndDock();
}