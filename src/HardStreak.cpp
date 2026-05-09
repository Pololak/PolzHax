#include "HardStreak.hpp"
#include "Setting.hpp"

void PolzHardStreak::updateStroke(float dt) { // This hook only works if calling convention is Thiscall! (edit HardStreak.hpp and HardStreak.cpp in build folder)
    if (setting().onWavePulseSize) this->m_pulseSize = setting().wavePulseSize;
    HardStreak::updateStroke(dt);
}

void PolzHardStreak::resumeStroke() {
    if (!setting().onWaveTrailBugFix) return HardStreak::resumeStroke();

    this->unschedule(schedule_selector(HardStreak::updateStroke));
    this->schedule(schedule_selector(HardStreak::updateStroke));
}