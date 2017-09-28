#include "RendererConfigWindow.h"
#include "ModuleRenderer3D.h"

RendererConfigWindow::RendererConfigWindow()
{
}

RendererConfigWindow::~RendererConfigWindow()
{
}

void RendererConfigWindow::DrawWindow()
{
	ImGui::Begin(window_name.c_str(), &active,
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_ShowBorders);

	ImGui::Checkbox("GL_LIGHTING", );

	ImGui::End();
}
