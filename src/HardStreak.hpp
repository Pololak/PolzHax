#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/HardStreak.hpp>

using namespace geode::prelude;

class PolzHardStreak : public geode::Modify<PolzHardStreak, HardStreak> {
public:
    void updateStroke(float);
    void resumeStroke();
};