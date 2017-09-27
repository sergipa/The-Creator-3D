#include "AppWindowConfigWindow.h"
#include "Application.h"

AppWindowConfigWindow::AppWindowConfigWindow()
{
	active = false;
	window_name = "Window Settings";
	fullscreen = false;
	//resizable = false;
	fullscren_desktop = false;
	brightness = 1.0f;
	width = App->window->GetWidth();
	height = App->window->GetHeight();
	fps_cap = 60;
}
AppWindowConfigWindow::~AppWindowConfigWindow()
{
	
}
void AppWindowConfigWindow::DrawWindow()
{
	ImGui::Begin(window_name.c_str(), &active,
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_ShowBorders);

	if (ImGui::Checkbox("Fullscreen", &fullscreen))
	{
		if (fullscreen)
			SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN);
		else 
			SDL_SetWindowFullscreen(App->window->window, 0);
	}

	if (ImGui::Checkbox("Fullscreen Desktop", &fullscreen))
	{
		if (fullscreen)
			SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		else
			SDL_SetWindowFullscreen(App->window->window, 0);
	}

	//if (ImGui::Checkbox("Resizable", &resizable)) Cant be changed while running, will implement when we can read json files
	//{
	//}


	ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f);
	if (ImGui::Button("Apply Brightness"))
	{
		SDL_SetWindowBrightness(App->window->window, brightness);
	}

	ImGui::SliderFloat("Width", &width, 100.0f, 1920.0f,"%.0f");
	ImGui::SliderFloat("Height", &height, 100.0f, 1080.0f,"%.0f");
	if (ImGui::Button("Apply Size"))
	{
		SDL_SetWindowSize(App->window->window, width, height);
	}

	ImGui::SliderFloat("FPS Limit", &fps_cap, 1.0f, 144.0f, "%.0f");
	if (ImGui::Button("Apply FPS Cap"))
	{
		App->CapFPS(fps_cap);
	}

	ImGui::End();
}
