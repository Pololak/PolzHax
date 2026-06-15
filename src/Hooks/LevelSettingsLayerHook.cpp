#include "LevelSettingsLayerHook.h"
#include "LevelEditorLayerHook.h"
#include "../GameVariables.h"

LevelSettingsLayer* m_levelSettingsLayer;

LevelSettingsLayer* LevelSettingsLayerHook::get() {
    return m_levelSettingsLayer;
}

void LevelSettingsLayerHook::Callback::togglePlaytestStartPos(CCObject*) {
    auto editorLayer = LevelEditorLayerHook::get();
    if (editorLayer) {
        if (LevelEditorLayerHook::getPlaytestStartPos() == static_cast<StartPosObject*>(editorLayer->m_uiLayer->m_selectedObject)) {
            LevelEditorLayerHook::setPlaytestStartPos(nullptr);
        }
        else {
            LevelEditorLayerHook::setPlaytestStartPos(static_cast<StartPosObject*>(editorLayer->m_uiLayer->m_selectedObject));
        }
    }
}

void LevelSettingsLayerHook::Callback::onPlaytestStartPos(CCObject*) {
    auto editorLayer = LevelEditorLayerHook::get();
    if (editorLayer) {
        this->onClose(nullptr);
        editorLayer->setStartPosObject(static_cast<StartPosObject*>(editorLayer->m_uiLayer->m_selectedObject));
        editorLayer->m_uiLayer->onPlaytest(nullptr);
    }
}

bool LevelSettingsLayerHook::initH(LevelSettingsLayer* self, LevelSettingsObject* settingsObject) {
    m_levelSettingsLayer = self;
    if (!LevelSettingsLayerHook::init(self, settingsObject)) return false;

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    if (self->m_customSongWidget) {
        CCSprite* toggleOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
        toggleOff->setScale(.7f);
        CCSprite* toggleOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
        toggleOn->setScale(.7f);

        auto onStartFlipped = CCMenuItemToggler::create(toggleOff, toggleOn, self, menu_selector(LevelSettingsLayer::onGravityFlipped));
        onStartFlipped->toggle(settingsObject->m_isFlipped);
        onStartFlipped->setPosition(self->m_buttonMenu->convertToNodeSpace({ winSize.width / 2.f - 135.f, winSize.height / 2.f - 135.f }));
        self->m_buttonMenu->addChild(onStartFlipped);
        auto onStartFlippedLabel = CCLabelBMFont::create("Start Flipped", "goldFont.fnt");
        onStartFlippedLabel->setAnchorPoint({ 0.f, .5f });
        onStartFlippedLabel->setScale(.5f);
        onStartFlippedLabel->setPosition(winSize.width / 2.f - 120.f, winSize.height / 2.f - 135.f);
        self->m_mainLayer->addChild(onStartFlippedLabel);

        // Blending dots

        CCPoint dotOffset = ccp(1.f, 51.f);

        auto line_blending = CCLabelBMFont::create(".", "bigFont.fnt");
        line_blending->setAnchorPoint({ 0.f, 1.f });
        line_blending->setPosition(dotOffset);
        self->m_lBtnSpr->addChild(line_blending); // lmao why not, even tho you can't change this

        auto dl_blending = CCLabelBMFont::create(".", "bigFont.fnt");
        dl_blending->setAnchorPoint({ 0.f, 1.f });
        dl_blending->setPosition(dotOffset);
        self->m_dlBtnSpr->addChild(dl_blending);

        auto col1_blending = CCLabelBMFont::create(".", "bigFont.fnt");
        col1_blending->setAnchorPoint({ 0.f, 1.f });
        col1_blending->setPosition(dotOffset);
        self->m_col1BtnSpr->addChild(col1_blending);

        auto col2_blending = CCLabelBMFont::create(".", "bigFont.fnt");
        col2_blending->setAnchorPoint({ 0.f, 1.f });
        col2_blending->setPosition(dotOffset);
        self->m_col2BtnSpr->addChild(col2_blending);

        auto col3_blending = CCLabelBMFont::create(".", "bigFont.fnt");
        col3_blending->setAnchorPoint({ 0.f, 1.f });
        col3_blending->setPosition(dotOffset);
        self->m_col3BtnSpr->addChild(col3_blending);

        auto col4_blending = CCLabelBMFont::create(".", "bigFont.fnt");
        col4_blending->setAnchorPoint({ 0.f, 1.f });
        col4_blending->setPosition(dotOffset);
        self->m_col4BtnSpr->addChild(col4_blending);

        dl_blending->setVisible(self->m_settingsObject->m_3DLineColor->m_blending);
        col1_blending->setVisible(self->m_settingsObject->m_customColor01->m_blending);
        col2_blending->setVisible(self->m_settingsObject->m_customColor02->m_blending);
        col3_blending->setVisible(self->m_settingsObject->m_customColor03->m_blending);
        col4_blending->setVisible(self->m_settingsObject->m_customColor04->m_blending);
    }

    if (settingsObject->m_startsWithStartPos) {
        auto playtestLabel = CCLabelBMFont::create("Playtest", "goldFont.fnt");
        playtestLabel->setScale(.6f);
        playtestLabel->setPosition(winSize.width / 2.f + 185.f, winSize.height / 2.f + 130.f);
        self->m_mainLayer->addChild(playtestLabel);

        auto onPlaytestStartPosSpr = CCSprite::createWithSpriteFrameName("GJ_playEditorBtn_001.png");
        auto onPlaytestStartPos = CCMenuItemSpriteExtra::create(onPlaytestStartPosSpr, self, menu_selector(LevelSettingsLayerHook::Callback::onPlaytestStartPos));
        onPlaytestStartPos->setPosition(185.f, 95.f);
        self->m_buttonMenu->addChild(onPlaytestStartPos);

        auto playtestFromLabel = CCLabelBMFont::create("Play from", "goldFont.fnt");
        playtestFromLabel->setScale(.6f);
        playtestFromLabel->setPosition(winSize.width / 2.f + 185.f, winSize.height / 2.f + 60.f);
        self->m_mainLayer->addChild(playtestFromLabel);

        bool playtestStartPosSelected = LevelEditorLayerHook::getPlaytestStartPos() != LevelEditorLayerHook::get()->m_uiLayer->m_selectedObject;
        self->createToggleButton("", menu_selector(LevelSettingsLayerHook::Callback::togglePlaytestStartPos), playtestStartPosSelected, self->m_buttonMenu, ccp(winSize.width / 2.f + 185.f, winSize.height / 2.f + 25.f));
    }

    return true;
}

void LevelSettingsLayerHook::colorSelectClosedH(LevelSettingsLayer* self, ColorSelectPopup* colorSelect) {
    LevelSettingsLayerHook::colorSelectClosed(self, colorSelect);

    auto dl_blending = static_cast<CCLabelBMFont*>(self->m_dlBtnSpr->getChildren()->objectAtIndex(0));
    auto col1_blending = static_cast<CCLabelBMFont*>(self->m_col1BtnSpr->getChildren()->objectAtIndex(0));
    auto col2_blending = static_cast<CCLabelBMFont*>(self->m_col2BtnSpr->getChildren()->objectAtIndex(0));
    auto col3_blending = static_cast<CCLabelBMFont*>(self->m_col3BtnSpr->getChildren()->objectAtIndex(0));
    auto col4_blending = static_cast<CCLabelBMFont*>(self->m_col4BtnSpr->getChildren()->objectAtIndex(0));

    if (dl_blending && col1_blending && col2_blending && col3_blending && col4_blending) {
        dl_blending->setVisible(self->m_settingsObject->m_3DLineColor->m_blending);
        col1_blending->setVisible(self->m_settingsObject->m_customColor01->m_blending);
        col2_blending->setVisible(self->m_settingsObject->m_customColor02->m_blending);
        col3_blending->setVisible(self->m_settingsObject->m_customColor03->m_blending);
        col4_blending->setVisible(self->m_settingsObject->m_customColor04->m_blending);
    }
}

void LevelSettingsLayerHook::selectArtClosedH(LevelSettingsLayer* self, SelectArtLayer* selectArtLayer) {
    LevelSettingsLayerHook::selectArtClosed(self, selectArtLayer);

    if (GameManager::sharedState()->getGameVariable(SHOW_GROUND)) {
        LevelEditorLayerHook::createGroundLayer();
    }
}

void LevelSettingsLayerHook::destructorH(LevelSettingsLayer* self) {
    LevelSettingsLayerHook::destructor(self);
    m_levelSettingsLayer = nullptr;
}

void LevelSettingsLayerHook::mem_init() {
    HOOK("_ZN18LevelSettingsLayer4initEP19LevelSettingsObject", LevelSettingsLayerHook::initH, LevelSettingsLayerHook::init);
    HOOK("_ZN18LevelSettingsLayer17colorSelectClosedEP16ColorSelectPopup", LevelSettingsLayerHook::colorSelectClosedH, LevelSettingsLayerHook::colorSelectClosed);
    HOOK("_ZN18LevelSettingsLayer15selectArtClosedEP14SelectArtLayer", LevelSettingsLayerHook::selectArtClosedH, LevelSettingsLayerHook::selectArtClosed);
    HOOK("_ZN18LevelSettingsLayerD0Ev", LevelSettingsLayerHook::destructorH, LevelSettingsLayerHook::destructor);
}