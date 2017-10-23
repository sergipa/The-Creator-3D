#include "TagsAndLayersWindow.h"
#include "Application.h"
#include "TagsAndLayers.h"
#include "Data.h"

TagsAndLayersWindow::TagsAndLayersWindow()
{
	active = false;
	window_name = "Tags & Layers";
}

TagsAndLayersWindow::~TagsAndLayersWindow()
{
}

void TagsAndLayersWindow::DrawWindow()
{
	ImGui::SetNextWindowSize({ 200, 100 }, ImGuiCond_Appearing);
	ImGui::Begin(window_name.c_str(), &active,
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_ShowBorders);

	if(ImGui::TreeNodeEx("Tags", ImGuiTreeNodeFlags_OpenOnArrow))
	{
		for (int i = 0; i < App->tags_and_layers->tags_list.size(); i++)
		{
			char tag_num[10];
			char tag_name[30];
			sprintf(tag_num, "Tag %d", i);
			sprintf(tag_name, "%s", App->tags_and_layers->tags_list[i].c_str());
			if (ImGui::InputText(tag_num, tag_name, 30, ImGuiInputTextFlags_EnterReturnsTrue) && i != 0)
			{
				App->tags_and_layers->AddTag(i, tag_name);
			}
			if (i < 1) continue;
			ImGui::SameLine();
			char delete_button_id[35];
			sprintf(delete_button_id, "-##%s", tag_name);
			if (ImGui::Button(delete_button_id, { 30,20 }))
			{
				App->tags_and_layers->RemoveTag(i);
			}
		}
		if (ImGui::Button("+##add_tag", { 30,20 }))
		{
			App->tags_and_layers->CreateEmptyTag();
		}

		ImGui::TreePop();
	}

	ImGui::Spacing();

	if (ImGui::TreeNodeEx("Layers", ImGuiTreeNodeFlags_OpenOnArrow))
	{
		for (int i = 0; i < App->tags_and_layers->layers_list.size(); i++)
		{
			char layer_num[10];
			char layer_name[30];
			sprintf(layer_num, "Layer %d", i);
			sprintf(layer_name, "%s", App->tags_and_layers->layers_list[i].c_str());
			if (ImGui::InputText(layer_num, layer_name, 30, ImGuiInputTextFlags_EnterReturnsTrue) && i > 2)
			{
				App->tags_and_layers->AddLayer(i, layer_name);
			}
			if (i < 3) continue;
			ImGui::SameLine();
			char delete_button_id[35];
			sprintf(delete_button_id, "-##%s", layer_name);
			if (ImGui::Button(delete_button_id, { 30,20 }))
			{
				App->tags_and_layers->RemoveLayer(i);
			}
		}
		if (ImGui::Button("+##add_layer", { 30,20 }))
		{
			App->tags_and_layers->CreateEmptyLayer();
		}

		ImGui::TreePop();
	}

	ImGui::End();
}
