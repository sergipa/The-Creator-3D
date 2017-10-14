#include "ConsoleWindow.h"

ConsoleWindow::ConsoleWindow()
{
	active = true;
	window_name = "Console";
	UpdateLabels();

	scroll_to_bottom = false;
	logs = 0;
	warnings = 0;
	errors = 0;
	debug_logs = 0;
	show_logs = true;
	show_warnings = true;
	show_errors = true;
	show_debug_logs = false;
	log_text_color = { 1.00f, 1.00f ,1.00f ,1.00f };
	warning_text_color = { 1.00f, 1.00f, 0.00f ,1.00f };
	error_text_color = { 1.00f, 0.00f, 0.00f ,1.00f };
	debug_text_color = { 0.40f, 0.90f, 0.90f ,1.00f };
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
		if (ImGui::Button(debug_label.c_str())) {
			show_debug_logs = !show_debug_logs;
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear")) {
			Clear();
		}

		ImGui::Separator();
		ImGui::BeginChild("scrolling");

		for (int i = 0; i < message_list.size(); i++) {
			if (show_errors && message_list[i].find("Error") != std::string::npos) {
				ImGui::TextColored(error_text_color, "%s", message_list[i].c_str());
			}
			else if (show_warnings && message_list[i].find("Warning") != std::string::npos) {
				ImGui::TextColored(warning_text_color, "%s", message_list[i].c_str());
			}
			else if (show_logs && message_list[i].find("Log") != std::string::npos) {
				ImGui::TextColored(log_text_color,"%s", message_list[i].c_str());
			}
			else if (show_debug_logs && message_list[i].find("Debug") != std::string::npos) {
				ImGui::TextColored(debug_text_color,"%s", message_list[i].c_str());
			}
			else {
				ImGui::Text("");
			}
		}

		if (scroll_to_bottom)
			ImGui::SetScrollHere(1.0f);
		scroll_to_bottom = false;
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
	debug_logs = 0;
	UpdateLabels();
}

void ConsoleWindow::AddLog(std::string log, bool is_wrror, bool is_warning, bool is_debug)
{
	std::string log_prefix;

	if (is_wrror) {
		log_prefix += " [Error]  ";
		if (errors < 99) {
			errors++;
		}
		else {
			message_list.erase(message_list.begin());
		}
	}
	else if (is_warning) {
		log_prefix += " [Warning]  ";
		if (warnings < 99) {
			warnings++;
		}
		else {
			message_list.erase(message_list.begin());
		}
	}
	else if (is_debug) {
		log_prefix += " [Debug]  ";
		if (debug_logs < 99) {
			debug_logs++;
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
	scroll_to_bottom = true;
	UpdateLabels();
}

void ConsoleWindow::UpdateLabels()
{
	errors_label = "Errors (" + std::to_string(errors) + ")";
	warnings_label = "Warnings (" + std::to_string(warnings) + ")";
	logs_label = "Logs (" + std::to_string(logs) + ")";
	debug_label = "Debug (" + std::to_string(debug_logs) + ")";
}
