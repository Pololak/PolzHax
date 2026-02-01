#pragma once
#include <string>
#include <imgui/imgui.h>
#include <string_view>
#include "Menu.hpp"

namespace ImGui {
	IMGUI_API bool HotKey(const char* label, int& key, float samelineOffset = 0.f, const ImVec2& size = ImVec2(100.f, 0.f), const char* placeholder = "...");
	IMGUI_API void Tooltip(const char* tooltip);
	IMGUI_API bool CheckboxF(const char* label, bool* v);
}