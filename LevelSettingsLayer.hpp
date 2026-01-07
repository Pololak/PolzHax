#pragma once
#include "pch.h"

namespace LevelSettingsLayer {
	inline bool(__thiscall* init)(gd::LevelSettingsLayer*, gd::LevelSettingsObject*);
	bool __fastcall initH(gd::LevelSettingsLayer*, void*, gd::LevelSettingsObject*);

	inline void(__thiscall* colorSelectClosed)(gd::LevelSettingsLayer*, gd::ColorSelectPopup*);
	void __fastcall colorSelectClosedH(gd::LevelSettingsLayer*, void*, gd::ColorSelectPopup*);

	void mem_init();
}