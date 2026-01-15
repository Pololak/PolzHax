#pragma once
#include "pch.h"

namespace UILayer {
	inline void(__thiscall* keyDown)(gd::UILayer*, enumKeyCodes);
	void __fastcall keyDownH(gd::UILayer*, void*, enumKeyCodes);

	inline void(__thiscall* keyUp)(gd::UILayer*, enumKeyCodes);
	void __fastcall keyUpH(gd::UILayer*, void*, enumKeyCodes);

	void mem_init();
}