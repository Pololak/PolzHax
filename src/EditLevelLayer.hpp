#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/EditLevelLayer.hpp>

using namespace geode::prelude;

class PolzEditLevelLayer : public geode::Modify<PolzEditLevelLayer, EditLevelLayer> {
public:
    struct Fields {
        CCLabelBMFont* m_orderLabel = nullptr;
        CCMenuItemSpriteExtra* m_orderUpBtn = nullptr;
        CCMenuItemSpriteExtra* m_orderDownBtn = nullptr;
        bool m_noRotationPass = false;
    };

    bool init(GJGameLevel*);
    void onEdit(CCObject*);
    void FLAlert_Clicked(FLAlertLayer*, bool);

    void updateLevelOrderLabel();

    void onMoveToTop(CCObject*);
    void onLevelOrderUp(CCObject*);
    void onLevelOrderDown(CCObject*);
};