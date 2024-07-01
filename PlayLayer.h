#pragma once
#include "pch.h"

namespace PlayLayer {

	inline bool(__thiscall* init)(gd::PlayLayer* self, gd::GJGameLevel* level);
	bool __fastcall init_H(gd::PlayLayer* self, void* edx, gd::GJGameLevel* level);
}