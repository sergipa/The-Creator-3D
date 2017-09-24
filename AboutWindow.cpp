#include "AboutWindow.h"


AboutWindow::AboutWindow()
{
	active = false;
	window_name = "About";
}

AboutWindow::~AboutWindow()
{
}

void AboutWindow::DrawWindow()
{
	ImGui::Begin(window_name.c_str(), &active,
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_ShowBorders);

	ImGui::Text("The Creator 0.2");
	ImGui::Text("The next generation 3D Game Engine");
	ImGui::Text("By Adria Martin & Sergi Perez");
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Text("3rd Party Libraries used:");
	ImGui::BulletText("SDL 2.0.6");
	ImGui::BulletText("SDL Mixer 2.0.0");
	ImGui::BulletText("Cereal 1.2.2");
	ImGui::BulletText("Glew 2.0.0");
	ImGui::BulletText("ImGui 1.51");
	ImGui::BulletText("MathGeoLib 1.5");
	ImGui::BulletText("OpenGL 2.1");
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Text("License: MIT");

	ImGui::End();
}
