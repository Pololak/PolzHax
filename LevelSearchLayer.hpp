#pragma once
#include "pch.h"

namespace LevelSearchLayer {
	inline gd::GJSearchObject* (__thiscall* getSearchObject)(gd::LevelSearchLayer*, gd::SearchType, std::string);
	gd::GJSearchObject* __fastcall getSearchObjectH(gd::LevelSearchLayer*, void*, gd::SearchType, std::string);

	void mem_init();
}