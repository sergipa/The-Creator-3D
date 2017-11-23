#include "TextEditorWindow.h"
#include <fstream>
#include <streambuf>
#include <dinput.h>
#include <tchar.h>

TextEditorWindow::TextEditorWindow()
{
	active = true;
	window_name = "TextEditor";

	editor.SetLanguageDefinition(TextEditor::LanguageDefinition::CSharp());
}


TextEditorWindow::~TextEditorWindow()
{
}

void TextEditorWindow::DrawWindow()
{
	auto cpos = editor.GetCursorPosition();

	if (ImGui::BeginDock(window_name.c_str(), false, false, false, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar))
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{
				std::string text_to_save = editor.GetText();
				/// save text in t....
			}
			if (ImGui::MenuItem("Quit", "Alt-F4"))
				this->active = false;
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			bool ro = editor.IsReadOnly();
			if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
				editor.SetReadOnly(ro);
			ImGui::Separator();

			if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && editor.CanUndo()))
				editor.Undo();
			if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && editor.CanRedo()))
				editor.Redo();

			ImGui::Separator();

			if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, editor.HasSelection()))
				editor.Copy();
			if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && editor.HasSelection()))
				editor.Cut();
			if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && editor.HasSelection()))
				editor.Delete();
			if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
				editor.Paste();

			ImGui::Separator();

			if (ImGui::MenuItem("Select all", nullptr, nullptr))
				editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(editor.GetTotalLines(), 0));

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();

		ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(),
			editor.IsOverwrite() ? "Ovr" : "Ins",
			editor.CanUndo() ? "*" : " ",
			editor.GetLanguageDefinition().mName.c_str(), path.c_str());

		editor.Render("TextEditor");

		ImGui::EndChild();
	}
	ImGui::EndDock();
}

void TextEditorWindow::SetPath(std::string string)
{
	std::ifstream t(string.c_str);
	std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	editor.SetText(str);
}
