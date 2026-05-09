#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>

using namespace geode::prelude;

class PolzLevelEditorLayer : public geode::Modify<PolzLevelEditorLayer, LevelEditorLayer> {
public:
    static LevelEditorLayer* get();

    bool init(GJGameLevel*);
    void updateVisibility(float);
    void update(float);
    void onPlaytest();
    void onResumePlaytest();

    void updateShowHitboxes();
    void updateOrientedHitboxes();
};