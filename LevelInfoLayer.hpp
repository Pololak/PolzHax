#pragma once
#include "pch.h"

namespace LevelInfoLayer {
	inline bool(__thiscall* init)(gd::LevelInfoLayer*, gd::GJGameLevel*);
	bool __fastcall initH(gd::LevelInfoLayer*, void*, gd::GJGameLevel*);

	inline void(__thiscall* onClone)(gd::LevelInfoLayer*, CCObject*);
	void __fastcall onCloneH(gd::LevelInfoLayer*, void*, CCObject*);

	inline void(__thiscall* songWidget)();
	void __fastcall songWidgetH();

	class Callback : public gd::LevelInfoLayer {
	public:
		void onExportLevel(CCObject*);
	};

	void mem_init();
}