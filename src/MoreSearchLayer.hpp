#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/MoreSearchLayer.hpp>

using namespace geode::prelude;

class PolzMoreSearchLayer : public geode::Modify<PolzMoreSearchLayer, MoreSearchLayer> {
public:
    bool init();
    void onExtraLong(CCObject*);
};