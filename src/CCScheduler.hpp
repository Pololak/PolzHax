#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/CCScheduler.hpp>

using namespace geode::prelude;

class PolzCCScheduler : public geode::Modify<PolzCCScheduler, CCScheduler> {
public:
    void update(float);
};