#include "RendererConfigWindow.h"
#include "ModuleRenderer3D.h"
#include "Application.h"
#include "OpenGL.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

RendererConfigWindow::RendererConfigWindow()
{
	active = false;
	window_name = "Renderer Config";

	SetDefaultValues();
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
	if (lighting)
	{
		App->renderer3D->EnableTestLight();
		ImGui::TextColored(ImVec4(0, 1, 0, 1), "Test Light");
		ImGui::DragFloat4("Ambient light", light_ambient, true, 0.025f, -1.0, 1.0);
		glLightfv(GL_LIGHT7, GL_AMBIENT, light_ambient);
		ImGui::DragFloat4("Difuse light", light_diffuse, true, 0.025f, -1.0, 1.0);
		glLightfv(GL_LIGHT7, GL_DIFFUSE, light_diffuse);
		ImGui::DragFloat4("Specular light", light_specular, true, 0.025f, -1.0, 1.0);
		glLightfv(GL_LIGHT7, GL_SPECULAR, light_specular);
		ImGui::DragFloat4("Light position", light_position, true, 0.025f);
		glLightfv(GL_LIGHT7, GL_POSITION, light_position);
		ImGui::DragFloat3("Light spot direction", light_spot_direction, true, 0.025f);
		glLightfv(GL_LIGHT7, GL_SPOT_DIRECTION, light_spot_direction);
		ImGui::DragFloat("Constant attenuation", &light_constant_attenuation, true, 0.025f, 0);
		glLightf(GL_LIGHT7, GL_CONSTANT_ATTENUATION, light_constant_attenuation);
		ImGui::DragFloat("Linear attenuation", &light_linear_attenuation, true, 0.025f, 0);
		glLightf(GL_LIGHT7, GL_LINEAR_ATTENUATION, light_linear_attenuation);
		ImGui::DragFloat("Quadratic attenuation", &light_quadratic_attenuation, true, 0.025f, 0);
		glLightf(GL_LIGHT7, GL_QUADRATIC_ATTENUATION, light_quadratic_attenuation);
	}

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
	if (color_material)
	{
		ImGui::TextColored(ImVec4(0, 1, 0, 1), "Test Color Material");
		const char* faces[] = { "Front", "Back", "Front & Back" };
		static int face = 2; //2 is the initial mode. 
		ImGui::Combo("Face", &face, faces, IM_ARRAYSIZE(faces));
		const char* modes[] = { "Emission", "Ambient", "Diffuse", "Specular", "Ambient & Diffuse" };
		static int mode = 4; //2 is the initial mode. 
		ImGui::Combo("Mode", &mode, modes, IM_ARRAYSIZE(modes));
		GLenum material_face;
		GLenum material_mode;
		switch (face)
		{
		case 0:
			material_face = GL_FRONT;
			break;
		case 1:
			material_face = GL_BACK;
			break;
		case 2:
			material_face = GL_FRONT_AND_BACK;
			break;
		}
		switch (mode)
		{
		case 0:
			material_mode = GL_EMISSION;
			break;
		case 1:
			material_mode = GL_AMBIENT;
			break;
		case 2:
			material_mode = GL_DIFFUSE;
			break;
		case 3:
			material_mode = GL_SPECULAR;
			break;
		case 4:
			material_mode = GL_AMBIENT_AND_DIFFUSE;
			break;
		}
		glColorMaterial(material_face, material_mode);
	}

	bool fog = App->renderer3D->GetActiveFog();
	ImGui::Checkbox("GL_FOG", &fog);
	App->renderer3D->SetActiveFog(fog);
	if (fog)
	{
		ImGui::TextColored(ImVec4(0, 1, 0, 1), "Test Fog");
		ImGui::SliderFloat4("Fog Color", fog_color, 0.0, 1.0);
		glFogfv(GL_FOG_COLOR, fog_color);
		ImGui::SliderFloat("Fog Density", &fog_density, 0.0f, 1.0f);
		glFogf(GL_FOG_DENSITY, fog_density);
		const char* modes[] = { "Linear", "EXP", "EXP2"};
		static int mode = 1; //1 = EXP Mode. This is the initial mode. 
		ImGui::Combo("Fog Mode", &mode, modes, IM_ARRAYSIZE(modes));
		glFogi(GL_FOG_MODE, mode);
	}

	if (!active)
	{
		SetDefaultValues();
		App->renderer3D->DisableTestLight();
	}

	ImGui::End();
}

void RendererConfigWindow::SetDefaultValues()
{

	//Fog
	fog_color[0] = 0.0f;
	fog_color[1] = 0.0f;
	fog_color[2] = 0.0f;
	fog_color[3] = 0.0f;
	fog_density = 1.0f;

	//Light
	light_ambient[0] = 1.0f;
	light_ambient[1] = 1.0f;
	light_ambient[2] = 1.0f;
	light_ambient[3] = 1.0f;

	light_diffuse[0] = 1.0f;
	light_diffuse[1] = 1.0f;
	light_diffuse[2] = 1.0f;
	light_diffuse[3] = 1.0f;

	light_specular[0] = 1.0f;
	light_specular[1] = 1.0f;
	light_specular[2] = 1.0f;
	light_specular[3] = 1.0f;

	light_constant_attenuation = 1.0f;
	light_linear_attenuation = 0.0f;
	light_quadratic_attenuation = 0.0f;

	light_position[0] = -1.0f;
	light_position[1] = 1.0f;
	light_position[2] = 1.0f;
	light_position[3] = 0.0f;

	light_spot_direction[0] = 0.0f;
	light_spot_direction[1] = 0.0f;
	light_spot_direction[2] = -1.0f;
}
