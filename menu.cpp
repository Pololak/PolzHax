#pragma warning(disable : 4996)
#include "pch.h"
#include "menu.h"

#include <imgui-hook.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include "state.h"

bool show = false;

ImVec4 color1;
ImVec4 color2;
ImVec4 color3;
ImVec4 color4;
ImVec4 color5;
ImVec4 color6;

void save() {
	auto file = fopen("Resources/polzsave.dat", "wb");
	if (file) {
		fwrite(&setting(), sizeof(setting()), 1, file);
		fclose(file);
	}
}

bool oneX = true;

void RenderMain() {
	unsigned long long pointervalue;
    if (oneX) {
        auto file = fopen("Resources/polzsave.dat", "rb");
        if (file) {
            fseek(file, 0, SEEK_END);
            auto size = ftell(file);

            if (size == sizeof(setting())) {
                fseek(file, 0, SEEK_SET);
                fread(&setting(), sizeof(setting()), 1, file);
                fclose(file);
            }
        }
        if (ImGui::Begin("Player", nullptr)) {
            ImGui::SetWindowPos({ 5,5 });
            ImGui::EndTabItem();
        }

        auto* colors = ImGui::GetStyle().Colors;

        color1.x = setting().Overlaycolor[0];
        color1.y = setting().Overlaycolor[1];
        color1.z = setting().Overlaycolor[2];
        color1.w = setting().Overlaycolor[3] - 0.4;

        color2.x = setting().Overlaycolor[0];
        color2.y = setting().Overlaycolor[1];
        color2.z = setting().Overlaycolor[2];
        color2.w = 1;

        color3.x = setting().Overlaycolor[0] + 0.3;
        color3.y = setting().Overlaycolor[1] + 0.3;
        color3.z = setting().Overlaycolor[2] + 0.3;
        color3.w = setting().Overlaycolor[3] + 0.3;

        color4.x = setting().Overlaycolor[0] - 0.1;
        color4.y = setting().Overlaycolor[1] - 0.1;
        color4.z = setting().Overlaycolor[2] - 0.1;
        color4.w = setting().Overlaycolor[3] - 0.1;

        color5.x = setting().Overlaycolor[0] + 0.1;
        color5.y = setting().Overlaycolor[1] + 0.1;
        color5.z = setting().Overlaycolor[2] + 0.1;
        color5.w = setting().Overlaycolor[3] + 0.1;

        color6.x = setting().BGcolor[0];
        color6.y = setting().BGcolor[1];
        color6.z = setting().BGcolor[2];
        color6.w = setting().BGcolor[3];

        //colors[ImGuiCol_Text] = ImVec4(1, 0, 0, 1);
        colors[ImGuiCol_WindowBg] = color6;
        colors[ImGuiCol_Border] = color2;
        colors[ImGuiCol_BorderShadow] = color2;
        colors[ImGuiCol_Button] = color3;
        colors[ImGuiCol_ButtonHovered] = color1;
        colors[ImGuiCol_ButtonActive] = color1;
        colors[ImGuiCol_CheckMark] = ImVec4(1, 1, 1, 1);
        colors[ImGuiCol_FrameBg] = color1;
        colors[ImGuiCol_FrameBgHovered] = color3;
        colors[ImGuiCol_FrameBgActive] = color3;
        colors[ImGuiCol_Tab] = ImVec4(color5.x, color5.y, color5.z, color5.w - 0.2);
        colors[ImGuiCol_TabHovered] = color3;
        colors[ImGuiCol_TabActive] = color3;
        colors[ImGuiCol_TitleBg] = color3;
        colors[ImGuiCol_TitleBgActive] = color3;
        colors[ImGuiCol_TitleBgCollapsed] = color3;
        colors[ImGuiCol_SliderGrab] = color4;
        colors[ImGuiCol_SliderGrabActive] = color4;
        colors[ImGuiCol_TextSelectedBg] = color2;
        colors[ImGuiCol_Header] = color1;
        colors[ImGuiCol_HeaderHovered] = color1;
        colors[ImGuiCol_HeaderActive] = color2;

        oneX = false;
    }

    if (show) {
        if (ImGui::Begin("Player", nullptr,
            ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize));
        {
            ImGui::SetWindowFontScale(setting().UISize);
            ImGui::SetNextItemWidth(120 * setting().UISize);

            if (ImGui::Checkbox("Noclip", &setting().onNoclip)) {
                if (setting().onNoclip) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F04E9), "\xe9\xf0\x02\x00\x00\x90", 6, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F04E9), "\x0f\x85\xef\x02\x00\x00", 6, NULL);
                }
            }
        }

        if (ImGui::Begin("Settings", nullptr,
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize));
        {
            ImGui::SetWindowFontScale(setting().UISize);
            if (ImGui::ColorEdit4("Color Style", setting().Overlaycolor)) {

                auto* colors = ImGui::GetStyle().Colors;

                color1.x = setting().Overlaycolor[0];
                color1.y = setting().Overlaycolor[1];
                color1.z = setting().Overlaycolor[2];
                color1.w = setting().Overlaycolor[3] - 0.4;

                color2.x = setting().Overlaycolor[0];
                color2.y = setting().Overlaycolor[1];
                color2.z = setting().Overlaycolor[2];
                color2.w = 1;

                color3.x = setting().Overlaycolor[0] + 0.3;
                color3.y = setting().Overlaycolor[1] + 0.3;
                color3.z = setting().Overlaycolor[2] + 0.3;
                color3.w = setting().Overlaycolor[3] + 0.3;

                color4.x = setting().Overlaycolor[0] - 0.1;
                color4.y = setting().Overlaycolor[1] - 0.1;
                color4.z = setting().Overlaycolor[2] - 0.1;
                color4.w = setting().Overlaycolor[3] - 0.1;

                color5.x = setting().Overlaycolor[0] + 0.1;
                color5.y = setting().Overlaycolor[1] + 0.1;
                color5.z = setting().Overlaycolor[2] + 0.1;
                color5.w = setting().Overlaycolor[3] + 0.1;

                //colors[ImGuiCol_Text] = ImVec4(0.3, 0.3, 0.3, 1);
                colors[ImGuiCol_WindowBg] = color6;
                colors[ImGuiCol_Border] = color2;
                colors[ImGuiCol_BorderShadow] = color2;
                colors[ImGuiCol_Button] = color3;
                colors[ImGuiCol_ButtonHovered] = color1;
                colors[ImGuiCol_ButtonActive] = color1;
                colors[ImGuiCol_CheckMark] = ImVec4(1, 1, 1, 1);
                colors[ImGuiCol_FrameBg] = color1;
                colors[ImGuiCol_FrameBgHovered] = color3;
                colors[ImGuiCol_FrameBgActive] = color3;
                colors[ImGuiCol_Tab] = ImVec4(color5.x, color5.y, color5.z, color5.w - 0.2);
                colors[ImGuiCol_TabHovered] = color3;
                colors[ImGuiCol_TabActive] = color3;
                colors[ImGuiCol_TitleBg] = color3;
                colors[ImGuiCol_TitleBgActive] = color3;
                colors[ImGuiCol_TitleBgCollapsed] = color3;
                colors[ImGuiCol_SliderGrab] = color4;
                colors[ImGuiCol_SliderGrabActive] = color4;
                colors[ImGuiCol_TextSelectedBg] = color2;
                colors[ImGuiCol_Header] = color1;
                colors[ImGuiCol_HeaderHovered] = color1;
                colors[ImGuiCol_HeaderActive] = color2;
            }

            if (ImGui::ColorEdit4("Background Color", setting().BGcolor)) {

                auto* colors = ImGui::GetStyle().Colors;

                color6.x = setting().BGcolor[0];
                color6.y = setting().BGcolor[1];
                color6.z = setting().BGcolor[2];
                color6.w = setting().BGcolor[3];

                colors[ImGuiCol_WindowBg] = color6;
            }


            ImGui::DragFloat("UI Size", &setting().UISize, 0.01f, 1.f, 3.25f);
            if (setting().UISize < 1.f) setting().UISize = 1.f;
            if (setting().UISize > 3.25f) setting().UISize = 3.25f;
        }
    }
    ImGui::End();
}

void setup_imgui_menu() {
    ImGuiHook::setToggleCallback([]() {show = !show; });
    ImGuiHook::setRenderFunction(RenderMain);
    ImGuiHook::setupHooks([](auto addr, auto hook, auto orig) {
        MH_CreateHook(addr, hook, orig);
        MH_EnableHook(addr);
        });
}
//void imgui_render() {
//	const bool force = state().just_loaded;
//	const auto& font = ImGui::GetIO().Fonts->Fonts.back();
//	ImGui::PushFont(font);
//	if (state().visible || force) {
//
//		// ImGui::ShowDemoWindow();
//		auto frame_size = CCDirector::sharedDirector()->getOpenGLView()->getFrameSize();
//
//		constexpr float border = 25;
//		ImGui::SetNextWindowPos({ border, border });
//		ImGui::SetNextWindowSizeConstraints({ 0, 0 }, { frame_size.width, frame_size.height - border * 2.f });
//
//		if (ImGui::Begin("mat's nice hacks", nullptr,
//			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar)) {
//
//		}
//	}
//		ImGui::End();