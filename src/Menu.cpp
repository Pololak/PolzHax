#include "Menu.hpp"
#include <Geode/Geode.hpp>
#include <imgui-cocos.hpp>
#include "Setting.hpp"
#include "ImGuiUtils.hpp"

using namespace geode::prelude;

ImVec4 color1;
ImVec4 color2;
ImVec4 color3;
ImVec4 color4;
ImVec4 color5;
ImVec4 color6;

bool oneX = true;

ImGuiTextFilter filter;

ImGuiTextFilter PolzHax::getTextFilter() {
	return filter;
}

const char* decimalPlaces[] = {
	"4 decimal places", "3 decimal places", "2 decimal places", "1 decimal place"
};

const int decimalValues[] = {
	4, 3, 2, 1
};

const char* priorities[] = {
	"Lowest", "Low", "Normal", "High", "Highest"
};

const char* statusLabelsPosition[] = {
	"Top-Left", "Top-Right", "Bottom-Right", "Bottom-Left"
};

void updatePriority() {
	switch (setting().priority) {
	case 0:
		SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS); break;
	case 1:
		SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS); break;
	case 2:
		SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS); break;
	case 3:
		SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS); break;
	case 4:
		SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS); break;
	}
}

void PolzHax::updateFPSBypass() {
	auto currentFps = setting().fpsValue;

	if (currentFps == 0.f) {
		currentFps = 60.f;
	}

	if (setting().onFPSBypass) {
		CCApplication::sharedApplication()->toggleVerticalSync(false);
		CCDirector::sharedDirector()->setAnimationInterval(1 / static_cast<double>(currentFps));
	}
	else {
		CCDirector::sharedDirector()->setAnimationInterval(1 / 60.0);
		if (GameManager::sharedState()->getGameVariable("0030")) {
			CCApplication::sharedApplication()->toggleVerticalSync(true);
		}
	}
}

void updateSpeedhack() {
	if (setting().speedhackValue == 0.f) return;

	const auto value = setting().onSpeedhack ? setting().speedhackValue : 1.f;

	CCDirector::sharedDirector()->m_pScheduler->setTimeScale(setting().onClassicMode ? 1.f : value);
	//SpeedHack::SetSpeed(setting().onClassicMode ? value : 1.f);

	if (auto fme = FMODAudioEngine::sharedEngine()) {
		if (auto sound = fme->m_globalChannel) {
			if (setting().onSpeedhackAudio) sound->setPitch(value);
			else sound->setPitch(1.f);
		}
	}
}

void colorSet() {
	auto* colors = ImGui::GetStyle().Colors;

	color1.x = setting().overlayColor01;
	color1.y = setting().overlayColor02;
	color1.z = setting().overlayColor03;
	color1.w = setting().overlayColor04 - 0.4f;

	color2.x = setting().overlayColor01;
	color2.y = setting().overlayColor02;
	color2.z = setting().overlayColor03;
	color2.w = 1;

	color3.x = setting().overlayColor01 + 0.3f;
	color3.y = setting().overlayColor02 + 0.3f;
	color3.z = setting().overlayColor03 + 0.3f;
	color3.w = setting().overlayColor04 + 0.3f;

	color4.x = setting().overlayColor01 - 0.1f;
	color4.y = setting().overlayColor02 - 0.1f;
	color4.z = setting().overlayColor03 - 0.1f;
	color4.w = setting().overlayColor04 - 0.1f;

	color5.x = setting().overlayColor01 + 0.1f;
	color5.y = setting().overlayColor02 + 0.1f;
	color5.z = setting().overlayColor03 + 0.1f;
	color5.w = setting().overlayColor04 + 0.1f;

	color6.x = setting().bgColor01;
	color6.y = setting().bgColor02;
	color6.z = setting().bgColor03;
	color6.w = setting().bgColor04;

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
	colors[ImGuiCol_Tab] = ImVec4(color5.x, color5.y, color5.z, color5.w - 0.2f);
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
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0, 0, 0, .33f);
	colors[ImGuiCol_ResizeGrip] = color2;
	colors[ImGuiCol_ResizeGripActive] = color5;
	colors[ImGuiCol_ResizeGripHovered] = color3;
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
}

void sortTabs() {
    float polzhax_xPos = 5.f;
	float addingInterfaceY = -1.f;
	float addingReplayY = -1.f;
	float bypass_xPos = -1.f;
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("PolzHax", nullptr);
		ImGui::SetWindowPos(ImVec2(5.f, 5.f));
		bypass_xPos = polzhax_xPos + ImGui::GetWindowWidth() + 5.f;
		addingInterfaceY = ImGui::GetWindowHeight() + 10.f;
	}
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Interface", nullptr);
		ImGui::SetWindowPos(ImVec2(5.f, addingInterfaceY));
		addingReplayY = addingInterfaceY + ImGui::GetWindowHeight() + 5.f;
	}
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Replay", nullptr);
		ImGui::SetWindowPos(ImVec2(5.f, addingReplayY));
	}
	float addingUtilityY = -1.f;
	float addingScreenshotY = -1.f;
	float cosmetic_xPos = -1.f;
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Bypass", nullptr);
		ImGui::SetWindowPos(ImVec2(bypass_xPos, 5.f));
		cosmetic_xPos = bypass_xPos + ImGui::GetWindowWidth() + 5.f;
		addingUtilityY = ImGui::GetWindowHeight() + 10.f;
	}
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Utility", nullptr);
		ImGui::SetWindowPos(ImVec2(bypass_xPos, addingUtilityY));
		addingScreenshotY = addingUtilityY + ImGui::GetWindowHeight() + 5.f;
	}
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Screenshot", nullptr);
		ImGui::SetWindowPos(ImVec2(bypass_xPos, addingScreenshotY));
	}
	float creator_xPos = -1.f;
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Cosmetic", nullptr);
		ImGui::SetWindowPos(ImVec2(cosmetic_xPos, 5.f));
		creator_xPos = cosmetic_xPos + ImGui::GetWindowWidth() + 5.f;
	}
	float addingVariablesY = -1.f;
	float level_xPos = -1.f;
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Creator", nullptr);
		ImGui::SetWindowPos(ImVec2(creator_xPos, 5.f));
		level_xPos = creator_xPos + ImGui::GetWindowWidth() + 5.f;
		addingVariablesY = ImGui::GetWindowHeight() + 10.f;
	}
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Variables", nullptr);
		ImGui::SetWindowPos(ImVec2(creator_xPos, addingVariablesY));
	}
	float universal_xPos = -1.f;
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Level", nullptr);
		ImGui::SetWindowPos(ImVec2(level_xPos, 5.f));
		universal_xPos = level_xPos + ImGui::GetWindowWidth() + 5.f;
	}
	float addingSpeedhackY = -1.f;
	float status_xPos = -1.f;
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Universal", nullptr);
		ImGui::SetWindowPos(ImVec2(universal_xPos, 5.f));
		status_xPos = universal_xPos + ImGui::GetWindowWidth() + 5.f;
		addingSpeedhackY = ImGui::GetWindowHeight() + 10.f;
	}
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Speedhack", nullptr);
		ImGui::SetWindowPos(ImVec2(universal_xPos, addingSpeedhackY));
	}
	float addingIconsY = -1.f;
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Status", nullptr);
		ImGui::SetWindowPos(ImVec2(status_xPos, 5.f));
		addingIconsY = ImGui::GetWindowHeight() + 10.f;
	}
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Icons", nullptr);
		ImGui::SetWindowPos(ImVec2(status_xPos, addingIconsY));
	}
}

void imgui_render() {
    auto playLayer = GameManager::sharedState()->getPlayLayer();
    auto editorLayer = 0;

    const float SHORT_ITEM_WIDTH = (ImGui::GetWindowWidth() / 2.f - ImGui::GetStyle().WindowPadding.x * 1.25f);
	const float LONG_ITEM_WIDTH = (ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x * 2.f);

    if (oneX) {
        sortTabs();
        colorSet();

        oneX = false;

		
    }

    if (setting().show) {
        ImGui::SetNextWindowSize(ImVec2(200.f, 0.f));
        if (ImGui::Begin("PolzHax", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {
			ImGui::Text("1.920 - v1.3.0 (Geode 060526)");

			ImGui::CheckboxF("Auto Save", &setting().onAutoSave);
			ImGui::SameLine(0.f, 0.f);
			ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2.f + (ImGui::GetStyle().WindowPadding.x / 4.f));
			if (ImGui::Button("Save", ImVec2(SHORT_ITEM_WIDTH, 0))) {
				setting().save();
				FLAlertLayer::create("Saved", "Hack state is saved.", "OK")->show();
			}

			ImGui::HotKey("Alt Hotkey", setting().m_openMenuKey, 0.f, ImVec2(SHORT_ITEM_WIDTH, 0));

			if (ImGui::CheckboxF("Thread Priority", &setting().onThreadPriority)) {
				if (setting().onThreadPriority) {
					updatePriority();
				}
				else {
					SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
				}
			}
			ImGui::SameLine(170.f);
			if (ImGui::TreeNodeEx("##threadPrioritySettings", ImGuiTreeNodeFlags_SpanAvailWidth)) {
				ImGui::SetNextItemWidth(163.f);
				if (ImGui::Combo("##priorities", &setting().priority, priorities, IM_ARRAYSIZE(priorities))) {
					if (setting().onThreadPriority) {
						updatePriority();
					}
				}
				ImGui::TreePop();
			}

            filter.Draw("Search", 135.f);

			if (setting().onDeveloperMode) {
				if (ImGui::Button("Cocos Explorer", ImVec2(LONG_ITEM_WIDTH, 0.f))) {
					setting().onCocosExplorer = !setting().onCocosExplorer;
				}

				if (ImGui::Button("GDPS Switcher", ImVec2(LONG_ITEM_WIDTH, 0.f))) {
					setting().onGDPSSwitcher = !setting().onGDPSSwitcher;
				}
			}

			if (GetAsyncKeyState(0x31) && GetAsyncKeyState(0x37) && GetAsyncKeyState(0x30) && GetAsyncKeyState(0x33)) { // Don't say anything about this
				ImGui::CheckboxF("Developer Mode", &setting().onDeveloperMode);
			}
        }
    }
}

void setupImGuiMenu() {
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    auto font = ImGui::GetIO().Fonts->AddFontFromFileTTF((Mod::get()->getResourcesDir() / "Muli-SemiBold.ttf").string().c_str(), 16.f);
    io.Fonts->Build();
    io.FontDefault = font;

    ImGui::GetStyle().WindowTitleAlign = ImVec2(.5f, .5f);
    ImGui::GetStyle().WindowBorderSize = 0;

	colorSet();

	PolzHax::updateFPSBypass();
}

$on_mod(Loaded) {
    ImGuiCocos::get().setup(setupImGuiMenu).draw(imgui_render).setVisible(setting().show);
}