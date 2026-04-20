#pragma once
#include "pch.h"

namespace LevelCell {
	inline void(__thiscall* loadLocalLevelCell)(gd::LevelCell*);
	void __fastcall loadLocalLevelCellH(gd::LevelCell*);

	std::vector<gd::GJGameLevel*> getSelectedLevels();

	class Callback : public gd::LevelCell {
	public:
		void onSelectLevel(CCObject*);
	};

	void mem_init();
}