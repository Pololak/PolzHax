#pragma once
#include "pch.h"
#include <imgui.h>

void setupImGuiMenu();

namespace PolzHax {
	ImGuiTextFilter getTextFilter();
	void updateFPSBypass();
}