#include <Windows.h>
#include <string>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGuiUtils.hpp"
#include <imgui/imgui_internal.h>

namespace StringH {
	std::string vkToString(int vk) {
#define caseStringify(x) case x: return std::string(#x + 3)
		char c[2] = { 0 };
		if (vk >= '0' && vk <= '9') { c[0] = (char)vk; return std::string(c); }
		if (vk >= 'A' && vk <= 'Z') { c[0] = (char)vk; return std::string(c); }
		switch (vk) {
		case VK_LBUTTON: return "LMB";
		case VK_RBUTTON: return "RMB";
			caseStringify(VK_CANCEL);
		case VK_MBUTTON: return "MMB";
		case VK_XBUTTON1: return "Mouse4";
		case VK_XBUTTON2: return "Mouse5";

			caseStringify(VK_BACK);
			caseStringify(VK_TAB);
			caseStringify(VK_CLEAR);
			caseStringify(VK_RETURN);
			caseStringify(VK_SHIFT);
			caseStringify(VK_CONTROL);
			caseStringify(VK_MENU);
			caseStringify(VK_PAUSE);
			caseStringify(VK_CAPITAL);
			caseStringify(VK_KANA);
			caseStringify(VK_JUNJA);
			caseStringify(VK_FINAL);
			caseStringify(VK_KANJI);
			caseStringify(VK_ESCAPE);
			caseStringify(VK_CONVERT);
			caseStringify(VK_NONCONVERT);
			caseStringify(VK_ACCEPT);
			caseStringify(VK_MODECHANGE);
			caseStringify(VK_SPACE);
			caseStringify(VK_PRIOR);
			caseStringify(VK_NEXT);
			caseStringify(VK_END);
			caseStringify(VK_HOME);
			caseStringify(VK_LEFT);
			caseStringify(VK_UP);
			caseStringify(VK_RIGHT);
			caseStringify(VK_DOWN);
			caseStringify(VK_SELECT);
			caseStringify(VK_PRINT);
			caseStringify(VK_EXECUTE);
			caseStringify(VK_SNAPSHOT);
			caseStringify(VK_INSERT);
			caseStringify(VK_DELETE);
			caseStringify(VK_HELP);
			caseStringify(VK_LWIN);
			caseStringify(VK_RWIN);
			caseStringify(VK_APPS);
			caseStringify(VK_SLEEP);
			caseStringify(VK_NUMPAD0);
			caseStringify(VK_NUMPAD1);
			caseStringify(VK_NUMPAD2);
			caseStringify(VK_NUMPAD3);
			caseStringify(VK_NUMPAD4);
			caseStringify(VK_NUMPAD5);
			caseStringify(VK_NUMPAD6);
			caseStringify(VK_NUMPAD7);
			caseStringify(VK_NUMPAD8);
			caseStringify(VK_NUMPAD9);
		case VK_MULTIPLY: return "*";
			caseStringify(VK_ADD);
			caseStringify(VK_SEPARATOR);
			caseStringify(VK_SUBTRACT);
			caseStringify(VK_DECIMAL);
		case VK_DIVIDE: return "/";
			caseStringify(VK_F1);
			caseStringify(VK_F2);
			caseStringify(VK_F3);
			caseStringify(VK_F4);
			caseStringify(VK_F5);
			caseStringify(VK_F6);
			caseStringify(VK_F7);
			caseStringify(VK_F8);
			caseStringify(VK_F9);
			caseStringify(VK_F10);
			caseStringify(VK_F11);
			caseStringify(VK_F12);
			caseStringify(VK_F13);
			caseStringify(VK_F14);
			caseStringify(VK_F15);
			caseStringify(VK_F16);
			caseStringify(VK_F17);
			caseStringify(VK_F18);
			caseStringify(VK_F19);
			caseStringify(VK_F20);
			caseStringify(VK_F21);
			caseStringify(VK_F22);
			caseStringify(VK_F23);
			caseStringify(VK_F24);
			caseStringify(VK_NUMLOCK);
			caseStringify(VK_SCROLL);
			caseStringify(VK_OEM_NEC_EQUAL);  // '=' key on numpad
			caseStringify(VK_OEM_FJ_MASSHOU); // 'Unregister word' key
			caseStringify(VK_OEM_FJ_TOUROKU); // 'Register word' key
			caseStringify(VK_OEM_FJ_LOYA);    // 'Left OYAYUBI' key
			caseStringify(VK_OEM_FJ_ROYA);    // 'Right OYAYUBI' key
			caseStringify(VK_LSHIFT);
			caseStringify(VK_RSHIFT);
			caseStringify(VK_LCONTROL);
			caseStringify(VK_RCONTROL);
		case VK_LMENU: return "LALT";
		case VK_RMENU: return "RALT";
			caseStringify(VK_BROWSER_BACK);
			caseStringify(VK_BROWSER_FORWARD);
			caseStringify(VK_BROWSER_REFRESH);
			caseStringify(VK_BROWSER_STOP);
			caseStringify(VK_BROWSER_SEARCH);
			caseStringify(VK_BROWSER_FAVORITES);
			caseStringify(VK_BROWSER_HOME);
			caseStringify(VK_VOLUME_MUTE);
			caseStringify(VK_VOLUME_DOWN);
			caseStringify(VK_VOLUME_UP);
			caseStringify(VK_MEDIA_NEXT_TRACK);
			caseStringify(VK_MEDIA_PREV_TRACK);
			caseStringify(VK_MEDIA_STOP);
			caseStringify(VK_MEDIA_PLAY_PAUSE);
			caseStringify(VK_LAUNCH_MAIL);
			caseStringify(VK_LAUNCH_MEDIA_SELECT);
			caseStringify(VK_LAUNCH_APP1);
			caseStringify(VK_LAUNCH_APP2);
			caseStringify(VK_OEM_1);      // ';:' for US
		case VK_OEM_PLUS: return "+";
		case VK_OEM_COMMA: return ",";
		case VK_OEM_MINUS: return "-";
		case VK_OEM_PERIOD: return ".";
		case VK_OEM_2: return "#";
			caseStringify(VK_OEM_3);  // '`~' for US
			caseStringify(VK_OEM_4);  //  '[{' for US
		case VK_OEM_5: return "^";
			caseStringify(VK_OEM_6);  //  ']}' for US
			caseStringify(VK_OEM_7);  //  ''"' for US
			caseStringify(VK_OEM_8);
			caseStringify(VK_OEM_AX);   //  'AX' key on Japanese AX kbd
		case VK_OEM_102: return "<";
			caseStringify(VK_ICO_HELP); //  Help key on ICO
			caseStringify(VK_ICO_00);   //  00 key on ICO
			caseStringify(VK_PROCESSKEY);
			caseStringify(VK_ICO_CLEAR);
			caseStringify(VK_PACKET);

		case 0: return "None";
		}

		// should not happen
		c[0] = (char)vk;
		return std::string(c);
	}
}

bool ImGui::HotKey(const char* label, int& key, float samelineOffset, const ImVec2& size, const char* placeholder) {
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems) return false;

	ImGuiContext& g = *GImGui;
	ImGuiIO& io = g.IO;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = CalcTextSize(label, NULL, true);

	//TextUnformatted(label);
	//SameLine(samelineOffset);
	Button(key == 0 ? placeholder : StringH::vkToString(key).c_str(), size);
	if (IsItemHovered()) {
		for (auto i = VK_MBUTTON; i <= VK_PACKET; i++) {
			switch (i) {
			case VK_ESCAPE:
			case VK_TAB:
			case VK_F1:
			case VK_OEM_3:
				continue;
			}

			if (GetAsyncKeyState(i) & 0x8000) {
				key = i;
			}

			if (IsItemClicked(ImGuiMouseButton_Right)) {
				key = 0;
			}
		}
	}

	SameLine(samelineOffset, 5.f);
	AlignTextToFramePadding();
	TextWrapped(label);

	return true;
}

void ImGui::Tooltip(const char* tooltip) {
	if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > .5f) {
		ImGui::SetTooltip(tooltip);
	}
}

bool ImGui::CheckboxF(const char* label, bool* v) {
	bool isDisabled = false;
	if (!PolzHax::getTextFilter().PassFilter(label)) isDisabled = true;
	ImGui::BeginDisabled(isDisabled);
	auto checkbox = ImGui::Checkbox(label, v);
	ImGui::EndDisabled();

	return checkbox;
}

bool ImGui::ButtonAligned(const char* label, float alignment) {
	ImGuiStyle& style = ImGui::GetStyle();

	float size = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.f;
	float avail = ImGui::GetContentRegionAvail().x;

	float off = (avail - size) * alignment;
	if (off > 0.f) {
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
	}

	return ImGui::Button(label);
}