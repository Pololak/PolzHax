#pragma once
#include "pch.h"

namespace EndLevelLayer {
	inline bool(__thiscall* customSetup)(gd::GJDropDownLayer* self);
	void __fastcall customSetup_H(gd::GJDropDownLayer* self);

	void mem_init();
}