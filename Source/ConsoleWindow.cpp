#include "ConsoleWindow.h"

ConsoleWindow::ConsoleWindow()
{
	active = true;
	window_name = "Console";
	UpdateLabels();
}

ConsoleWindow::~ConsoleWindow()
{
}

void ConsoleWindow::DrawWindow()
{
	if (ImGui::BeginDock(window_name.c_str(), false, false, false, ImGuiWindowFlags_HorizontalScrollbar)) {
		if (ImGui::Button(logs_label.c_str())) {
			show_logs = !show_logs;
		}
		ImGui::SameLine();
		if (ImGui::Button(warnings_label.c_str())) {
			show_warnings = !show_warnings;
		}
		ImGui::SameLine();
		if (ImGui::Button(errors_label.c_str())) {
			show_errors = !show_errors;
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear")) {
			Clear();
		}

		ImGui::Separator();
		ImGui::BeginChild("scrolling");
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));

		for (int i = 0; i < message_list.size(); i++) {
			if (show_errors && message_list[i].find("Error") != std::string::npos) {
				ImGui::TextColored(error_text_color, "%s", message_list[i].c_str());
			}
			else if (show_warnings && message_list[i].find("Warning") != std::string::npos) {
				ImGui::TextColored(warning_text_color, "%s", message_list[i].c_str());
			}
			else if (show_logs && message_list[i].find("Log") != std::string::npos) {
				ImGui::Text("%s", message_list[i].c_str());
			}
			else {
				ImGui::Text("");
			}
		}

		if (scroll_to_bottom)
			ImGui::SetScrollHere(1.0f);
		scroll_to_bottom = false;
		ImGui::PopStyleVar();
		ImGui::EndChild();
	}
	ImGui::EndDock();
}

void ConsoleWindow::Clear()
{
	message_list.clear();
	logs = 0;
	warnings = 0;
	errors = 0;
	UpdateLabels();
}

void ConsoleWindow::AddLog(std::string log, bool isError, bool isWarning)
{
	std::string log_prefix;

	if (isError) {
		log_prefix += " [Error]  ";
		if (errors < 99) {
			errors++;
		}
		else {
			message_list.erase(message_list.begin());
		}
	}
	else if (isWarning) {
		log_prefix += " [Warning]  ";
		if (warnings < 99) {
			warnings++;
		}
		else {
			message_list.erase(message_list.begin());
		}
	}
	else {
		log_prefix += " [Log]  ";
		if (logs < 99) {
			logs++;
		}
		else {
			message_list.erase(message_list.begin());
		}
	}

	log_prefix += log;
	message_list.push_back(log_prefix);
	message_list.push_back("");
	scroll_to_bottom = true;
	UpdateLabels();
}

void ConsoleWindow::UpdateLabels()
{
	errors_label = "Errors (" + std::to_string(errors) + ")";
	warnings_label = "Warnings (" + std::to_string(warnings) + ")";
	logs_label = "Logs (" + std::to_string(logs) + ")";
}
