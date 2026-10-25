#pragma once
#include "pch.h"

namespace HardStreak {
	inline void(__thiscall* updateStroke)(gd::HardStreak*, float);
	void __fastcall updateStrokeH(gd::HardStreak*, void*, float);

	inline void(__thiscall* resumeStreak)(gd::HardStreak*);
	void __fastcall resumeStreakH(gd::HardStreak*);

	inline void(__thiscall* addPoint)(gd::HardStreak*, CCPoint);
	void __fastcall addPointH(gd::HardStreak*, void*, CCPoint);

	void mem_init();
}