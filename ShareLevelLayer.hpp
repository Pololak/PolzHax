#pragma once
#include "pch.h"

namespace ShareLevelLayer {
	inline bool(__thiscall* init)(gd::ShareLevelLayer*, gd::GJGameLevel*);
	bool __fastcall initH(gd::ShareLevelLayer*, void*, gd::GJGameLevel*);

	void mem_init();
}