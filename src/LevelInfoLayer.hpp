#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

class PolzLevelInfoLayer : public geode::Modify<PolzLevelInfoLayer, LevelInfoLayer> {
public:
    bool init(GJGameLevel*);
    void FLAlert_Clicked(FLAlertLayer*, bool);

    void onMoveToTop(CCObject*);
};