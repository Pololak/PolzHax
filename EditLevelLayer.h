#pragma once
#include "pch.h"

namespace EditLevelLayer
{
	inline bool(__thiscall* EditLevelLayer_init)(gd::EditLevelLayer* self, gd::GJGameLevel* gjlvl);
	bool __fastcall EditLevelLayer_init_H(gd::EditLevelLayer* self, void*, gd::GJGameLevel* gjlvl);
}