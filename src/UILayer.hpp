#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/UILayer.hpp>

using namespace geode::prelude;

class PolzUILayer : public geode::Modify<PolzUILayer, UILayer> {
public:
    bool init();
    void keyDown(enumKeyCodes);
    void keyUp(enumKeyCodes);

    void onPrevStartPos(CCObject*);
    void onNextStartPos(CCObject*);
};