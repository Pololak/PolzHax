#pragma once
#include "pch.h"

namespace CCSchedulerHook {
	inline void(__thiscall* update)(cocos2d::CCScheduler*, float);
	void __fastcall updateH(cocos2d::CCScheduler*, void*, float);

	void setNextFrame(bool);

	void mem_init();
}

