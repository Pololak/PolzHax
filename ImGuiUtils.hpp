#pragma once
#include <string>
#include <imgui/imgui.h>
#include <string_view>
#include <vector>
#include "Menu.hpp"

namespace ImGui {
	IMGUI_API bool HotKey(const char* label, int& key, float samelineOffset = 0.f, const ImVec2& size = ImVec2(100.f, 0.f), const char* placeholder = "...");
	IMGUI_API void Tooltip(const char* tooltip);
	IMGUI_API bool CheckboxF(const char* label, bool* v);
	IMGUI_API bool ButtonAligned(const char* label, float alignment = .5f);
	//IMGUI_API bool Combo(const char* label, int* current_item, const std::vector<std::string>& items, int items_count, int height_in_items = -1);
	IMGUI_API bool Combo(const char* label, int* current_item, const std::vector<std::string>& items, int items_count, int height_in_items = -1);
}