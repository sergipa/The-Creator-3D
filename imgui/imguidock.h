#ifndef IMGUIDOCK_H_
#define IMGUIDOCK_H_

// based on https://github.com/nem0/LumixEngine/blob/master/external/imgui/imgui_dock.h
// Lumix Engine Dock. From: https://github.com/nem0/LumixEngine/blob/master/src/editor/imgui/imgui_dock.h
/*
The MIT License (MIT)
Copyright (c) 2013-2016 Mikulas Florek
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
// modified from https://bitbucket.org/duangle/liminal/src/tip/src/liminal/imgui_dock.h


#ifndef IMGUI_API
#include  "imgui.h"
#endif //IMGUI_API

typedef enum ImGuiDockSlot {
	ImGuiDockSlot_Left = 0,
	ImGuiDockSlot_Right,
	ImGuiDockSlot_Top,
	ImGuiDockSlot_Bottom,
	ImGuiDockSlot_Tab,

	ImGuiDockSlot_Float,
	ImGuiDockSlot_None
} ImGuiDockSlot;

namespace ImGui {

	IMGUI_API void BeginDockspace();
	IMGUI_API void EndDockspace();
	IMGUI_API void ShutdownDock();
	IMGUI_API void SetNextDock(ImGuiDockSlot slot);
	IMGUI_API bool BeginDock(const char* label, bool noTab = false, bool* opened = NULL, bool locked = false, ImGuiWindowFlags extra_flags = 0, const ImVec2& default_size = ImVec2(-1, -1));
	IMGUI_API void EndDock();
	IMGUI_API void SetDockActive();
	IMGUI_API void DockDebugWindow();
	IMGUI_API void SaveDocks();
	IMGUI_API void LoadDocks();

} // namespace ImGui

extern bool gImGuiDockReuseTabWindowTextureIfAvailable; // [true] (used only when available)


#endif //IMGUIDOCK_H_