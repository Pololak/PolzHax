#include "HardStreak.hpp"
#include "Setting.hpp"

void __fastcall HardStreak::updateStrokeH(gd::HardStreak* self, void*, float dt) {
	if (setting().onWavePulseSize) self->m_pulseSize = setting().wavePulseSize;
	HardStreak::updateStroke(self, dt);
}

void __fastcall HardStreak::resumeStreakH(gd::HardStreak* self) {
	if (!setting().onWaveTrailBugFix) return HardStreak::resumeStreak(self);

	self->unschedule(schedule_selector(gd::HardStreak::updateStroke));
	self->schedule(schedule_selector(gd::HardStreak::updateStroke));
}

void HardStreak::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x833e0), HardStreak::updateStrokeH, reinterpret_cast<void**>(&HardStreak::updateStroke));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x832d0), HardStreak::resumeStreakH, reinterpret_cast<void**>(&HardStreak::resumeStreak));
}