#include "AssetsWindow.h"
#include "Application.h"
//#include "tinyfiledialogs.h"
#include "ModuleEditor.h"

AssetsWindow::AssetsWindow()
{
	active = true;
	window_name = "Assets";

}

AssetsWindow::~AssetsWindow()
{

}

void AssetsWindow::DrawWindow()
{
	if (ImGui::BeginDock(window_name.c_str(), false, false/*, App->IsPlaying()*/, ImGuiWindowFlags_HorizontalScrollbar)) {
		ImGui::Columns(2);

		ImGui::NextColumn();

		if (ImGui::BeginChild("Files", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar/*, App->IsPlaying()*/)) {

		}
		ImGui::EndChild();
	}
	ImGui::EndDock();
}
