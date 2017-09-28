#include "RendererConfigWindow.h"
#include "ModuleRenderer3D.h"
#include "Application.h"

RendererConfigWindow::RendererConfigWindow()
{
	active = false;
	window_name = "Renderer Config";
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

	bool lighting = App->renderer3D->GetActiveLighting();
	ImGui::Checkbox("GL_LIGHTING", &lighting);
	App->renderer3D->SetActiveLighting(lighting);

	bool depth_test = App->renderer3D->GetActiveDepthTest();
	ImGui::Checkbox("GL_DEPTH_TEST", &depth_test);
	App->renderer3D->SetActiveDepthTest(depth_test);

	bool texture = App->renderer3D->GetActiveTexture2D();
	ImGui::Checkbox("GL_TEXTURE2D", &texture);
	App->renderer3D->SetActiveTexture2D(texture);

	bool cull_test = App->renderer3D->GetActiveCullTest();
	ImGui::Checkbox("GL_CULL_TEST", &cull_test);
	App->renderer3D->SetActiveCullTest(cull_test);

	bool color_material = App->renderer3D->GetActiveColorMaterial();
	ImGui::Checkbox("GL_COLOR_MATERIAL", &color_material);
	App->renderer3D->SetActiveColorMaterial(color_material);

	bool fog = App->renderer3D->GetActiveFog();
	ImGui::Checkbox("GL_FOG", &fog);
	App->renderer3D->SetActiveFog(fog);
	if (fog)
	{
		GLfloat color[4] = { 1.0,0.0,1.0,1.0 };
		//ImGui::DragFloat4("Fog Color", color,0.25f);
		glFogfv(GL_FOG_COLOR, color);
	}

	ImGui::End();
}
