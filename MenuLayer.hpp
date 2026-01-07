#pragma once
#include "pch.h"

namespace MenuLayer {
	inline bool(__thiscall* init)(gd::MenuLayer*);
	bool __fastcall initH(gd::MenuLayer*, void*);

	void mem_init();
}