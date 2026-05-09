#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class PolzPauseLayer : public geode::Modify<PolzPauseLayer, PauseLayer> {
public:
    static PauseLayer* get();

    void customSetup();
    void onEdit(cocos2d::CCObject*);
    void onResume(cocos2d::CCObject*);
    void onQuit(cocos2d::CCObject*);
    void onRestart(cocos2d::CCObject*);
};