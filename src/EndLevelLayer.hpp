#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/EndLevelLayer.hpp>

using namespace geode::prelude;

class PolzEndLevelLayer : public geode::Modify<PolzEndLevelLayer, EndLevelLayer> {
public:
    struct Fields {
        CCLabelBMFont* m_cheatIndicator = nullptr;
    };

    void customSetup();
    void updateCheatIndicator2();
};