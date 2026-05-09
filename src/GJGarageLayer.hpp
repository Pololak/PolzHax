#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/GJGarageLayer.hpp>

using namespace geode::prelude;

class PolzGJGarageLayer : public geode::Modify<PolzGJGarageLayer, GJGarageLayer> {
public:
    bool init();
    void onIconPreview(CCObject*);
};