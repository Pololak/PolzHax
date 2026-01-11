#pragma once
#include <string>
#include <imgui/imgui.h>
#include <string_view>

namespace ImGui {
	IMGUI_API bool HotKey(const char* label, int& key, float samelineOffset = 0.f, const ImVec2& size = ImVec2(100.f, 0.f), const char* placeholder = "...");
	IMGUI_API void Tooltip(const char* tooltip);
}