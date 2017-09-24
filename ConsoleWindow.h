#pragma once
#include "Window.h"
#include <vector>

class ConsoleWindow :
	public Window
{
public:
	ConsoleWindow();
	virtual ~ConsoleWindow();

	void DrawWindow();
	void Clear();
	void AddLog(std::string log, bool isError = false, bool isWarning = false);
	void UpdateLabels();

private:
	ImGuiTextBuffer	buf;
	ImGuiTextFilter filter;
	ImVector<int> line_offsets;
	bool scroll_to_bottom = false;
	int logs = 0;
	int warnings = 0;
	int errors = 0;
	std::string logs_label;
	std::string warnings_label;
	std::string errors_label;
	bool show_logs = true;
	bool show_warnings = true;
	bool show_errors = true;
	std::vector<std::string> message_list;
	ImVec4 log_text_color = { 1.00f, 1.00f ,1.00f ,1.00f };
	ImVec4 warning_text_color = { 1.00f, 1.00f, 0.00f ,1.00f };
	ImVec4 error_text_color = { 1.00f, 0.00f, 0.00f ,1.00f };
};

