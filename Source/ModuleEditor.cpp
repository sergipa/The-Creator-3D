#include "ModuleEditor.h"
#include "Application.h"
#include "imgui_impl_sdl.h"
#include "HierarchyWindow.h"
#include "PropertiesWindow.h"
#include "AssetsWindow.h"
#include "ConsoleWindow.h"
#include "SceneWindow.h"
#include "ModuleWindow.h"
#include "HardwareWindow.h"
#include "PerformanceWindow.h"
#include "AboutWindow.h"
#include "AppWindowConfigWindow.h"
#include "EditorStyleWindow.h"
#include "Data.h"
#include "RendererConfigWindow.h"
#include "InputConfigWindow.h"

ModuleEditor::ModuleEditor(Application * app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Editor";
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init(Data* editor_config)
{
	ImGui_ImplSdl_Init(App->window->window);

	// (there is a default font, this is only if you want to change it. see extra_fonts/README.txt for more details)
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
	io.Fonts->AddFontFromFileTTF(EDITOR_FONTS_FOLDER"Cousine-Regular.ttf", 15.0f);
	io.Fonts->AddFontFromFileTTF(EDITOR_FONTS_FOLDER"DroidSans.ttf", 16.0f);
	io.Fonts->AddFontFromFileTTF(EDITOR_FONTS_FOLDER"ProggyClean.ttf", 13.0f);
	io.Fonts->AddFontFromFileTTF(EDITOR_FONTS_FOLDER"ProggyTiny.ttf", 10.0f);
	font = io.Fonts->AddFontFromFileTTF(EDITOR_FONTS_FOLDER"OpenSans-Semibold.ttf", 16.0f);

	LoadEditorStyle();
	
	editor_windows.push_back(scene_window = new SceneWindow());
	editor_windows.push_back(assets_window = new AssetsWindow());
	editor_windows.push_back(hierarchy_window = new HierarchyWindow());
	editor_windows.push_back(properties_window = new PropertiesWindow());
	editor_windows.push_back(console_window = new ConsoleWindow());
	editor_windows.push_back(hardware_window = new HardwareWindow());
	editor_windows.push_back(performance_window = new PerformanceWindow());
	editor_windows.push_back(about_window = new AboutWindow());
	editor_windows.push_back(config_window = new AppWindowConfigWindow());
	editor_windows.push_back(style_editor_window = new EditorStyleWindow());
	editor_windows.push_back(renderer_config_window = new RendererConfigWindow());
	editor_windows.push_back(input_config_window = new InputConfigWindow());
	//editor_panels.push_back(animator_panel = new PanelAnimator());
	//editor_panels.push_back(particle_editor_panel = new PanelParticleEditor());
	ImGui::LoadDocks();
	return true;
}

update_status ModuleEditor::PreUpdate(float delta_time)
{
	ImGui_ImplSdl_NewFrame(App->window->window);
	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update(float deltaTime)
{
	ms_timer.Start();
	bool ret = true;
	ImGui::PushFont(font);
	if (ImGui::BeginMainMenuBar())
	{
		bool selected = false;
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4 previous_color = style.Colors[ImGuiCol_Text];
		style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		if (ImGui::BeginMenu("File"))
		{
			style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

			if (ImGui::MenuItem("Exit")) {
				return UPDATE_STOP;
			}

			ImGui::EndMenu();
			style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		}

		if (ImGui::BeginMenu("Windows"))
		{
			style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			if (ImGui::BeginMenu("Configuration"))
			{
				if (ImGui::MenuItem("Window Config"))
				{
					config_window->active = !config_window->active;
				}
				if (ImGui::MenuItem("Renderer Config"))
				{
					renderer_config_window->active = !renderer_config_window->active;
				}
				if (ImGui::MenuItem("Input Config"))
				{
					input_config_window->active = !input_config_window->active;
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Hardware"))
			{
				hardware_window->active = !hardware_window->active;
			}
			if (ImGui::MenuItem("Performance"))
			{
				performance_window->active = !performance_window->active;
			}
			if (ImGui::MenuItem("Editor Style"))
			{
				style_editor_window->active = !style_editor_window->active;
			}
			ImGui::EndMenu();
			style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		}

		if (ImGui::BeginMenu("Help"))
		{
			style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			if (ImGui::MenuItem("About..."))
			{
				about_window->active = !about_window->active;
			}
			if (ImGui::MenuItem("Documentation"))
			{
				OpenBrowserPage("https://github.com/sergipa/The-Creator-3D/wiki");
			}
			if (ImGui::MenuItem("Download Latest"))
			{
				OpenBrowserPage("https://github.com/sergipa/The-Creator-3D/releases");
			}
			if (ImGui::MenuItem("Report a bug"))
			{
				OpenBrowserPage("https://github.com/sergipa/The-Creator-3D/issues");
			}
			ImGui::EndMenu();
			style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		}
		ImGui::EndMainMenuBar();
		style.Colors[ImGuiCol_Text] = previous_color;
	}

	ImVec2 display_size = ImGui::GetIO().DisplaySize;
	display_size.y -= 10;
	ImGui::SetNextWindowSize(display_size);
	ImGui::SetNextWindowPos(ImVec2(0, 20));
	ImGui::Begin("PanelEditor", NULL, ImVec2(0, 0), 1.0f, ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);

	ImGui::SameLine(ImGui::GetIO().DisplaySize.x / 2 - 75);
	if (ImGui::Button("Play", { 50,40 })) {
		//App->Play();
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop", { 50,40 })) {
		//App->Stop();
	}
	ImGui::SameLine();
	if (ImGui::Button("Pause", { 50,40 })) {

	}

	ImGui::Separator();
	ImGui::BeginDockspace();
	for (std::list<Window*>::iterator it = editor_windows.begin(); it != editor_windows.end(); it++) {
		if ((*it)->IsActive())
		{
			(*it)->DrawWindow();
		}
	}
	ImGui::EndDockspace();
	ImGui::End();
	ImGui::PopFont();
	performance_window->AddModuleData(this->name, ms_timer.ReadMs());
	return UPDATE_CONTINUE;
}

bool ModuleEditor::DrawEditor()
{
	ImGui::Render();
	return true;
}

bool ModuleEditor::CleanUp()
{
	for (std::list<Window*>::iterator it = editor_windows.begin(); it != editor_windows.end(); ++it) {
		RELEASE(*it);
	}
	editor_windows.clear();
	ImGui::SaveDocks();
	ImGui_ImplSdl_Shutdown();
	return true;
}

void ModuleEditor::HandleInput(SDL_Event * event)
{
	ImGui_ImplSdl_ProcessEvent(event);
}

void ModuleEditor::OpenBrowserPage(const char * url)
{
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}

void ModuleEditor::AddData_Editor(float ms, float fps)
{
	performance_window->AddData(ms, fps);
}

void ModuleEditor::LoadEditorStyle()
{
	Data data;

	if (data.LoadJSON(EDITOR_STYLE_FILE))
	{
		ImGuiStyle * style = &ImGui::GetStyle();
		data.EnterSection("Editor_Style");
		style->Alpha = data.GetFloat("Alpha");
		style->WindowPadding = data.GetVector2("WindowPadding");
		style->WindowMinSize = data.GetVector2("WindowMinSize");
		style->WindowRounding = data.GetFloat("WindowRounding");
		style->WindowTitleAlign = data.GetVector2("WindowTitleAlign");
		style->ChildWindowRounding = data.GetFloat("ChildWindowRounding");
		style->FramePadding = data.GetVector2("FramePadding");
		style->FrameRounding = data.GetFloat("FrameRounding");
		style->ItemSpacing = data.GetVector2("ItemSpacing");
		style->ItemInnerSpacing = data.GetVector2("ItemInnerSpacing");
		style->TouchExtraPadding = data.GetVector2("TouchExtraPadding");
		style->IndentSpacing = data.GetFloat("IndentSpacing");
		style->ColumnsMinSpacing = data.GetFloat("ColumnsMinSpacing");
		style->ScrollbarSize = data.GetFloat("ScrollbarSize");
		style->ScrollbarRounding = data.GetFloat("ScrollbarRounding");
		style->GrabMinSize = data.GetFloat("GrabMinSize");
		style->GrabRounding = data.GetFloat("GrabRounding");
		style->ButtonTextAlign = data.GetVector2("ButtonTextAlign");
		style->DisplayWindowPadding = data.GetVector2("DisplayWindowPadding");
		style->DisplaySafeAreaPadding = data.GetVector2("DisplaySafeAreaPadding");
		style->AntiAliasedLines = data.GetBool("AntiAliasedLines");             
		style->AntiAliasedShapes = data.GetBool("AntiAliasedShapes");
		style->CurveTessellationTol = data.GetFloat("CurveTessellationTol");

		for (int i = 0; i < ImGuiCol_COUNT; i++)
		{
			style->Colors[i] = data.GetVector4(ImGui::GetStyleColorName(i));
		}
		data.LeaveSection();
	}
}

