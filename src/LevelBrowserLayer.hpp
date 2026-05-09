#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>

using namespace geode::prelude;

class PolzLevelBrowserLayer : public geode::Modify<PolzLevelBrowserLayer, LevelBrowserLayer> {
public:
    bool init(GJSearchObject*);
    void FLAlert_Clicked(FLAlertLayer*, bool);
    
    void onDeleteSelected(CCObject*);
};