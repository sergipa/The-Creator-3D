#include "InputConfigWindow.h"
#include "ModuleCamera3D.h"
#include "Application.h"
#include "ModuleInput.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

InputConfigWindow::InputConfigWindow()
{
	active = false;
	window_name = "Input Config";
}


InputConfigWindow::~InputConfigWindow()
{
}

void InputConfigWindow::DrawWindow()
{
	ImGui::Begin(window_name.c_str(), &active,
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_ShowBorders);

	ImGui::Text("Mouse Position:");
	ImGui::Text("X: %d", App->input->GetMouseX());
	ImGui::Text("Y: %d", App->input->GetMouseY());
	ImGui::Text("Mouse Motion:");
	ImGui::Text("X: %d", App->input->GetMouseXMotion());
	ImGui::Text("Y: %d", App->input->GetMouseYMotion());
	ImGui::Text("Mouse Wheel: %d", App->input->GetMouseZ());

	ImGui::Separator();

	const char* Keys[] = { "A" ,"B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R",
		"S", "T", "U", "V", "W", "X", "Y", "Z", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
		"ESC", "L_CTRL", "L_SHIFT", "L_ALT", "R_CTRL", "R_SHIFT", "R_ALT", "MENU", "[", "]", ";", ",", ".","'", "/", "BACK_SLASH",
		"~", "=", "-", "SPACE", "RETURN", "BACK_SPACE", "TAB", "PAGE_UP", "PAGE_DOWN", "END","HOME","INSERT","DELETE"
		"+", "-", "*", "/", "LEFT_ARROW", "RIGHT_ARROW", "UP_ARROW","DOWN_ARROW","NUM_PAD_0","NUM_PAD_1",
		"NUM_PAD_2", "NUM_PAD_3", "NUM_PAD_4", "NUM_PAD_5", "NUM_PAD_6", "NUM_PAD_7", "NUM_PAD_8", "NUM_PAD_9", "F1",
		"F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10","F11","F12","F13","F14","F15","PAUSE" };

	ImGui::Text("Camera:");

	static int key_speed = SearchInKeys(Keys,App->input->KeyToString(App->camera->key_speed));//DEFAULT LSHIFT
	static int key_forward = SearchInKeys(Keys, App->input->KeyToString(App->camera->key_forward));;//DEFAULT W
	static int key_backward = SearchInKeys(Keys, App->input->KeyToString(App->camera->key_backward));;//DEFAULT S
	static int key_up = SearchInKeys(Keys, App->input->KeyToString(App->camera->key_up));;//DEFAULT Q
	static int key_down = SearchInKeys(Keys, App->input->KeyToString(App->camera->key_down));;//DEFAULT E
	static int key_left = SearchInKeys(Keys, App->input->KeyToString(App->camera->key_left));;//DEFAULT A
	static int key_right = SearchInKeys(Keys, App->input->KeyToString(App->camera->key_right));;//DEFAULT D


	ImGui::Combo("key_speed", &key_speed, Keys, IM_ARRAYSIZE(Keys));
	ImGui::Combo("key_forward", &key_forward, Keys, IM_ARRAYSIZE(Keys));
	ImGui::Combo("key_backward", &key_backward, Keys, IM_ARRAYSIZE(Keys));
	ImGui::Combo("key_up", &key_up, Keys, IM_ARRAYSIZE(Keys));
	ImGui::Combo("key_down", &key_down, Keys, IM_ARRAYSIZE(Keys));
	ImGui::Combo("key_left", &key_left, Keys, IM_ARRAYSIZE(Keys));
	ImGui::Combo("key_right", &key_right, Keys, IM_ARRAYSIZE(Keys));

	App->camera->key_speed = App->input->StringToKey(Keys[key_speed]);
	App->camera->key_forward = App->input->StringToKey(Keys[key_forward]);
	App->camera->key_backward = App->input->StringToKey(Keys[key_backward]);
	App->camera->key_up = App->input->StringToKey(Keys[key_up]);
	App->camera->key_down = App->input->StringToKey(Keys[key_down]);
	App->camera->key_left = App->input->StringToKey(Keys[key_left]);
	App->camera->key_right = App->input->StringToKey(Keys[key_right]);

	ImGui::End();
	
}

int InputConfigWindow::SearchInKeys(const char *keys[],std::string key)
{
	int temp;
	for (temp = 0; temp < 99; temp++)
	{
		if (key.compare(keys[temp]) == 0) return temp;
	}

	return temp;
}
