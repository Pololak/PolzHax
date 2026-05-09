#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/CustomSongWidget.hpp>

using namespace geode::prelude;

class PolzCustomSongWidget : public geode::Modify<PolzCustomSongWidget, CustomSongWidget> {
public:
    struct Fields {
        CCMenuItemSpriteExtra* m_copySongIDBtn = nullptr;
    };

    bool init(SongInfoObject*, LevelSettingsObject*, bool, bool, bool, bool, bool);
    //void updateSongInfo();

    void onCopySongID(CCObject*);
};