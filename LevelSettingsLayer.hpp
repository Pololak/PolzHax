#pragma once
#include "pch.h"

namespace LevelSettingsLayer {
	inline bool(__thiscall* init)(gd::LevelSettingsLayer*, gd::LevelSettingsObject*);
	bool __fastcall initH(gd::LevelSettingsLayer*, void*, gd::LevelSettingsObject*);

	inline void(__thiscall* colorSelectClosed)(gd::LevelSettingsLayer*, gd::ColorSelectPopup*);
	void __fastcall colorSelectClosedH(gd::LevelSettingsLayer*, void*, gd::ColorSelectPopup*);

	inline void(__thiscall* selectArtClosed)(gd::LevelSettingsLayer*, gd::SelectArtLayer*);
	void __fastcall selectArtClosedH(gd::LevelSettingsLayer*, void*, gd::SelectArtLayer*);

	inline void(__thiscall* destructor)(gd::LevelSettingsLayer*);
	void __fastcall destructorH(gd::LevelSettingsLayer*);

	gd::LevelSettingsLayer* get();

	class Callback : public gd::LevelSettingsLayer {
	public:
		void onPlaytestStartPos(CCObject*);
	};

	void mem_init();
}