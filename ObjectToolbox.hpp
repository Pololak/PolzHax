#pragma once
#include "pch.h"

namespace ObjectToolbox {
	inline float(__thiscall* gridNodeSizeForKey)(gd::ObjectToolbox*, int);
	float __fastcall gridNodeSizeForKeyH(gd::ObjectToolbox*, void*, int);

	void mem_init();
}