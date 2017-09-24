#include "HardwareWindow.h"
#include "SDL\include\SDL.h"

HardwareWindow::HardwareWindow()
{
	active = false;
	window_name = "Hardware";
}

HardwareWindow::~HardwareWindow()
{
}

void HardwareWindow::DrawWindow()
{
	ImGui::Begin(window_name.c_str(), &active,
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_ShowBorders);

	SDL_version version;
	SDL_GetVersion(&version);

	ImGui::Text("SDL Version: ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.%d.%d", version.major, version.minor, version.patch);
	ImGui::Separator();
	ImGui::Text("CPUs: ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d (Cache: %dkb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
	ImGui::Text("RAM: ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%.3fGb ", SDL_GetSystemRAM() / 1024.f);
	ImGui::Text("Caps: ");
	ImGui::SameLine();
	std::string str;
	if (SDL_Has3DNow) str += "3DNow, ";
	if (SDL_HasAVX) str += "AVX, ";
	if (SDL_HasAVX2) str += "AVX2, ";
	if (SDL_HasAltiVec) str += "AltiVec, ";
	if (SDL_HasMMX) str += "MMX, ";
	if (SDL_HasRDTSC) str += "RDTSC, ";
	if (SDL_HasSSE) str += "SSE, ";
	if (SDL_HasSSE2) str += "SSE2, ";
	if (SDL_HasSSE3) str += "SSE3, ";
	if (SDL_HasSSE41) str += "SSE41, ";
	if (SDL_HasSSE41) str += "SSE42";
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", str.c_str());

	ImGui::End();
}
