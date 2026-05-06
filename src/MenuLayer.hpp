#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

struct PolzMenuLayer : geode::Modify<PolzMenuLayer, MenuLayer> {
    bool init();
};