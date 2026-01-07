#pragma once
#include "pch.h"

namespace EditButtonBar {
	inline bool(__thiscall* init)(gd::EditButtonBar*, CCArray*, int, bool, CCPoint);
	bool __fastcall initH(gd::EditButtonBar*, void*, CCArray*, int, bool, CCPoint);

	void mem_init();
}