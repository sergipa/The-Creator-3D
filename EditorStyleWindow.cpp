#include "EditorStyleWindow.h"


EditorStyleWindow::EditorStyleWindow()
{
	active = false;
	window_name = "Style Editor";
}

EditorStyleWindow::~EditorStyleWindow()
{
}

void EditorStyleWindow::DrawWindow()
{
	ImGui::Begin(window_name.c_str(), &active,
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_ShowBorders);

	ImGui::ShowStyleEditor();

	ImGui::End();
}
