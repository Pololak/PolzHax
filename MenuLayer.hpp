#pragma once
#include "pch.h"

namespace MenuLayer {
	inline bool(__thiscall* init)(gd::MenuLayer*);
	bool __fastcall initH(gd::MenuLayer*, void*);

	inline void(__thiscall* onMoreGames)(gd::MenuLayer*, CCObject*);
	void __fastcall onMoreGamesH(gd::MenuLayer*, void*, CCObject*);

	void mem_init();
}