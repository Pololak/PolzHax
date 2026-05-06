#pragma once
#include <../imgui/imgui.h>

void setupImGuiMenu();

namespace PolzHax {
    ImGuiTextFilter getTextFilter();
    void updateFPSBypass();
}