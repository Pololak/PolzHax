#include "EditorPauseLayerHook.h"
#include "LevelEditorLayerHook.h"
#include "EditorUIHook.h"
#include "../GameVariables.h"
#include "../utils.h"
#include "../Setting.h"

bool EditorPauseLayerHook::initH(EditorPauseLayer* self, LevelEditorLayer* editorLayer) {
    if (!EditorPauseLayerHook::init(self, editorLayer)) return false;
    LevelEditorLayerHook::setIsEditorPaused(true);

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    auto levelTimeLabel = CCLabelBMFont::create("", "goldFont.fnt");
    levelTimeLabel->setScale(.5f);
    levelTimeLabel->setPosition(director->getScreenLeft() + 10.f, director->getScreenTop() - 25.f);
    levelTimeLabel->setAnchorPoint({ 0.f, 1.f });
    self->addChild(levelTimeLabel);

    // Time logic
    float time = floorf(editorLayer->m_gridLayer->timeForXPos(editorLayer->getLastObjectX()));
    int hours = time / 3600;
    int minutes = (time - (hours * 3600)) / 60;
    int seconds = (time - (minutes * 60)) - (hours * 3600);

    if (time > 3599) {
        levelTimeLabel->setString(CCString::createWithFormat("%ih %im %is", hours, minutes, seconds)->getCString());
    }
    else if (time > 59) {
        levelTimeLabel->setString(CCString::createWithFormat("%imin %is", minutes, seconds)->getCString());
    }
    else {
        levelTimeLabel->setString(CCString::createWithFormat("%is", seconds)->getCString());
    }
    //

    return true;
}

void EditorPauseLayerHook::Callback::onSave(CCObject*) {
    auto layer = FLAlertLayer::create(this, "Save", "<cy>Save</c> the level?", "NO", "YES", 300.f);
    layer->setTag(2);
    layer->show();
}

void EditorPauseLayerHook::Callback::onSelectAll(CCObject*) {
    auto editorLayer = this->m_levelEditorLayer;
    auto editorUI = editorLayer->m_uiLayer;

    auto objects = CCArray::create();
    CCARRAY_FOREACH_B_TYPE(editorLayer->m_levelSections, section, CCArray) {
        if (section) {
            CCARRAY_FOREACH_B_TYPE(section, object, GameObject) {
                if (object && object->m_editorGroup == editorLayer->m_groupIDFilter || editorLayer->m_groupIDFilter == -1) {
                    objects->addObject(object);
                }
            }
        }
    }

    editorUI->createUndoSelectObject(false);
    editorUI->selectObjects(objects);
    editorUI->updateButtons();
    editorUI->deactivateRotationControl();
}

void selectAllWithDirection(bool rightDir) {
    auto editorLayer = LevelEditorLayerHook::get();

    if (editorLayer) {
        auto editorUI = editorLayer->m_uiLayer;

        auto cameraPos = editorLayer->m_gameLayer->getPosition();
        auto cameraScale = editorLayer->m_gameLayer->getScale();
        int centerX = -(cameraPos.x) / cameraScale + CCDirector::sharedDirector()->getWinSize().width / 2;

        auto objects = CCArray::create();
        CCARRAY_FOREACH_B_TYPE(editorLayer->m_levelSections, section, CCArray) {
            if (section) {
                CCARRAY_FOREACH_B_TYPE(section, object, GameObject) {
                    if (object && (object->m_editorGroup == editorLayer->m_groupIDFilter) || editorLayer->m_groupIDFilter == -1) {
                        if ((rightDir && object->getPositionX() >= centerX) || (!rightDir && object->getPositionX() <= centerX)) {
                            objects->addObject(object);
                        }
                    }
                }
            }
        }

        editorUI->createUndoSelectObject(false);
        editorUI->selectObjects(objects);
        editorUI->updateButtons();
        editorUI->deactivateRotationControl();
    }
}

void alignObjects(bool alignY) {
    auto editorUI = EditorUIHook::get();

    if (editorUI) {
        auto selectedObjects = editorUI->m_selectedObjects;
        int objectCount = selectedObjects->count();
        auto randomKid = reinterpret_cast<GameObject*>(selectedObjects->objectAtIndex(0));

        if (objectCount < 3) return;

        editorUI->m_editorLayer->addToUndoList(UndoObject::createWithTransformObjects(selectedObjects, UndoCommand::Transform), false);

        std::vector<GameObject*> sortedArray;
        CCARRAY_FOREACH_B_TYPE(selectedObjects, object, GameObject) {
            if (object) {
                sortedArray.push_back(object);
            }
        }

        if (!alignY) {
            float minX = randomKid->getPositionX();
            float maxX = randomKid->getPositionX();

            CCARRAY_FOREACH_B_TYPE(selectedObjects, object, GameObject) {
                if (object) {
                    float compareX = object->getPositionX();
                    if (compareX < minX) {
                        minX = compareX;
                    }
                    if (compareX > maxX) {
                        maxX = compareX;
                    }
                }
            }

            std::sort(sortedArray.begin(), sortedArray.end(), [](GameObject* a, GameObject* b) {
                return a->getPositionX() < b->getPositionX();
                });
            float step = (maxX - minX) / (objectCount - 1);
            float currentStep = 0.f;

            for (auto obj : sortedArray) {
                if (obj) {
                    editorUI->moveObject(obj, ccp(minX + currentStep - obj->getPositionX(), 0));
                    currentStep += step;
                }
            }
        }
        else {
            float minY = randomKid->getPositionY();
            float maxY = randomKid->getPositionY();

            CCARRAY_FOREACH_B_TYPE(selectedObjects, object, GameObject) {
                if (object) {
                    float compareY = object->getPositionY();
                    if (compareY < minY) {
                        minY = compareY;
                    }
                    if (compareY > maxY) {
                        maxY = compareY;
                    }
                }
            }

            std::sort(sortedArray.begin(), sortedArray.end(), [](GameObject* a, GameObject* b) {
                return a->getPositionY() < b->getPositionY();
                });
            float step = (maxY - minY) / (objectCount - 1);
            float currentStep = 0.f;

            for (auto obj : sortedArray) {
                if (obj) {
                    editorUI->moveObject(obj, ccp(0, minY + currentStep - obj->getPositionY()));
                    currentStep += step;
                }
            }
        }
    }
}

void EditorPauseLayerHook::Callback::onSelectAllLeft(CCObject*) {
    selectAllWithDirection(false);
}

void EditorPauseLayerHook::Callback::onSelectAllRight(CCObject*) {
    selectAllWithDirection(true);
}

void EditorPauseLayerHook::Callback::onAlignY(CCObject*) {
	alignObjects(true);
}

void EditorPauseLayerHook::Callback::onAlignX(CCObject*) {
	alignObjects(false);
}

void EditorPauseLayerHook::Callback::onSmallEditorStep(CCObject*) {
    GameManager::sharedState()->toggleGameVariable("0035");
}

void EditorPauseLayerHook::Callback::onSelectFilter(CCObject*) {
    GameManager::sharedState()->toggleGameVariable(SELECT_FILTER);
}

void EditorPauseLayerHook::Callback::onShowObjectInfo(CCObject*) {
    auto gm = GameManager::sharedState();
    gm->toggleGameVariable(SHOW_OBJECT_INFO);
    auto editorUI = this->m_levelEditorLayer->m_uiLayer;
    if (editorUI) {
        auto objectInfoLabel = static_cast<CCLabelBMFont*>(editorUI->getChildByTag(2701));
        if (objectInfoLabel) {
            objectInfoLabel->setVisible(gm->getGameVariable(SHOW_OBJECT_INFO));
            EditorUIHook::updateObjectInfoLabel(editorUI);
        }
    }
}

void EditorPauseLayerHook::Callback::onShowGround(CCObject*) {
    auto gm = GameManager::sharedState();
    gm->toggleGameVariable(SHOW_GROUND);
    if (gm->getGameVariable(SHOW_GROUND)) {
        LevelEditorLayerHook::createGroundLayer();
    }
    else {
        LevelEditorLayerHook::removeGroundLayer();
    }
}

void EditorPauseLayerHook::Callback::onPreviewMode(CCObject*) {
    auto gm = GameManager::sharedState();
    gm->toggleGameVariable(PREVIEW_MODE);
    if (gm->getGameVariable(PREVIEW_MODE)) {
        LevelEditorLayerHook::updatePreviewMode();
    }
    else {
        LevelEditorLayerHook::resetColors();
    }
}

void EditorPauseLayerHook::Callback::onResetStartPos(CCObject*) {
    if (LevelEditorLayerHook::getPlaytestStartPos()) {
        LevelEditorLayerHook::setPlaytestStartPos(nullptr);
    }
}

void EditorPauseLayerHook::Callback::onEditorOptions(CCObject*) {
    
}

void EditorPauseLayerHook::customSetupH(EditorPauseLayer* self) {
    EditorPauseLayerHook::customSetup(self);

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    auto mainMenu = static_cast<CCMenu*>(self->getChildren()->objectAtIndex(0));
    mainMenu->setAnchorPoint({ .0f, .0f });
    mainMenu->setScale(.9f);

    auto onSaveSpr = ButtonSprite::create("Save", 0xdc, 0, 1.f, true, "goldFont.fnt", "GJ_button_01.png", 30.f);
    auto onSave = CCMenuItemSpriteExtra::create(onSaveSpr, self, menu_selector(EditorPauseLayerHook::Callback::onSave));

    static_cast<CCMenuItemSpriteExtra*>(mainMenu->getChildren()->objectAtIndex(0))->setPositionY(90.f);
    static_cast<CCMenuItemSpriteExtra*>(mainMenu->getChildren()->objectAtIndex(1))->setPositionY(45.f);
    static_cast<CCMenuItemSpriteExtra*>(mainMenu->getChildren()->objectAtIndex(2))->setPositionY(0.f);
    onSave->setPositionY(-45.f);
    static_cast<CCMenuItemSpriteExtra*>(mainMenu->getChildren()->objectAtIndex(3))->setPositionY(-90.f);

    mainMenu->addChild(onSave);

    auto bottom_menu = static_cast<CCMenu*>(self->m_audioOnBtn->getParent());

    auto onSelectAllRightSpr = ButtonSprite::create("Select\nAll Right", 0x1e, 0, 1.f, true, "bigFont.fnt", "GJ_button_04.png", 30.f);
    auto onSelectAllRight = CCMenuItemSpriteExtra::create(onSelectAllRightSpr, self, menu_selector(EditorPauseLayerHook::Callback::onSelectAllRight));
    onSelectAllRight->setPosition(bottom_menu->convertToNodeSpace({winSize.width - 115.f, director->getScreenBottom() + 30.f}));
    bottom_menu->addChild(onSelectAllRight);

    auto onSelectAllLeftSpr = ButtonSprite::create("Select\nAll Left", 0x1e, 0, 1.f, true, "bigFont.fnt", "GJ_button_04.png", 30.f);
    auto onSelectAllLeft = CCMenuItemSpriteExtra::create(onSelectAllLeftSpr, self, menu_selector(EditorPauseLayerHook::Callback::onSelectAllLeft));
    onSelectAllLeft->setPosition(bottom_menu->convertToNodeSpace({ winSize.width - 115.f, director->getScreenBottom() + 65.f }));
    bottom_menu->addChild(onSelectAllLeft);

    auto onSelectAllSpr = ButtonSprite::create("Select\nAll", 0x1e, 0, 1.f, true, "bigFont.fnt", "GJ_button_04.png", 30.f);
    auto onSelectAll = CCMenuItemSpriteExtra::create(onSelectAllSpr, self, menu_selector(EditorPauseLayerHook::Callback::onSelectAll));
    onSelectAll->setPosition(bottom_menu->convertToNodeSpace({ winSize.width - 115.f, director->getScreenBottom() + 100.f }));
    bottom_menu->addChild(onSelectAll);

    auto onAlignYSpr = ButtonSprite::create("Align Y", 0x1e, 0, 1.f, true, "bigFont.fnt", "GJ_button_04.png", 30.f);
    auto onAlignY = CCMenuItemSpriteExtra::create(onAlignYSpr, self, menu_selector(EditorPauseLayerHook::Callback::onAlignY));
    onAlignY->setPosition(bottom_menu->convertToNodeSpace({ winSize.width - 115.f, director->getScreenBottom() + 135.f }));
    bottom_menu->addChild(onAlignY);

    auto onAlignXSpr = ButtonSprite::create("Align X", 0x1e, 0, 1.f, true, "bigFont.fnt", "GJ_button_04.png", 30.f);
    auto onAlignX = CCMenuItemSpriteExtra::create(onAlignXSpr, self, menu_selector(EditorPauseLayerHook::Callback::onAlignX));
    onAlignX->setPosition(bottom_menu->convertToNodeSpace({ winSize.width - 115.f, director->getScreenBottom() + 170.f }));
    bottom_menu->addChild(onAlignX);

    auto gm = GameManager::sharedState();

    GameToolbox::createToggleButton(
        "Small Editor Step",
        menu_selector(EditorPauseLayerHook::Callback::onSmallEditorStep),
        gm->getGameVariable("0035"),
        bottom_menu,
        ccp(30.f, director->getScreenBottom() + 120.f),
        self, self,
        .7f, .4f, 80.f,
        ccp(8.f, 0.f),
        "bigFont.fnt",
        false
    );

    GameToolbox::createToggleButton(
        "Select Filter",
        menu_selector(EditorPauseLayerHook::Callback::onSelectFilter),
        gm->getGameVariable(SELECT_FILTER),
        bottom_menu,
        ccp(30.f, director->getScreenBottom() + 150.f),
        self, self,
        .7f, .4f, 80.f,
        ccp(8.f, 0.f),
        "bigFont.fnt",
        false
    );

    GameToolbox::createToggleButton(
        "Show Object Info",
        menu_selector(EditorPauseLayerHook::Callback::onShowObjectInfo),
        gm->getGameVariable(SHOW_OBJECT_INFO),
        bottom_menu,
        ccp(30.f, director->getScreenBottom() + 180.f),
        self, self,
        .7f, .4f, 80.f,
        ccp(8.f, 0.f),
        "bigFont.fnt",
        false
    );

    GameToolbox::createToggleButton(
        "Show Ground",
        menu_selector(EditorPauseLayerHook::Callback::onShowGround),
        gm->getGameVariable(SHOW_GROUND),
        bottom_menu,
        ccp(30.f, director->getScreenBottom() + 210.f),
        self, self,
        .7f, .4f, 80.f,
        ccp(8.f, 0.f),
        "bigFont.fnt",
        false
    );

    GameToolbox::createToggleButton(
        "Preview Mode",
        menu_selector(EditorPauseLayerHook::Callback::onPreviewMode),
        gm->getGameVariable(PREVIEW_MODE),
        bottom_menu,
        ccp(30.f, director->getScreenBottom() + 240.f),
        self, self,
        .7f, .4f, 80.f,
        ccp(8.f, 0.f),
        "bigFont.fnt",
        false
    );

    auto onResetStartPosSpr = ButtonSprite::create("Reset\nStartPos", 0x32, 0, .4f, true, "bigFont.fnt", "GJ_button_04.png", 30.f);
    auto onResetStartPos = CCMenuItemSpriteExtra::create(onResetStartPosSpr, self, menu_selector(EditorPauseLayerHook::Callback::onResetStartPos));
    onResetStartPos->setPosition(bottom_menu->convertToNodeSpace({ director->getScreenRight() - 50.f, director->getScreenBottom() + 65.f }));
    bottom_menu->addChild(onResetStartPos);
}

void EditorPauseLayerHook::FLAlert_ClickedH(EditorPauseLayer* self, FLAlertLayer* layer, bool btn2) {
    if ((layer->getTag() == 2) && btn2) {
        self->saveLevel();
    }
    EditorPauseLayerHook::FLAlert_Clicked(self, layer, btn2);
}

void EditorPauseLayerHook::destructorH(EditorPauseLayer* self) {
    LevelEditorLayerHook::setIsEditorPaused(false);
    EditorPauseLayerHook::destructor(self);
}

void EditorPauseLayerHook::mem_init() {
    HOOK("_ZN16EditorPauseLayer4initEP16LevelEditorLayer", EditorPauseLayerHook::initH, EditorPauseLayerHook::init);
    HOOK("_ZN16EditorPauseLayer11customSetupEv", EditorPauseLayerHook::customSetupH, EditorPauseLayerHook::customSetup);
    HOOK("_ZN16EditorPauseLayer15FLAlert_ClickedEP12FLAlertLayerb", EditorPauseLayerHook::FLAlert_ClickedH, EditorPauseLayerHook::FLAlert_Clicked);
    HOOK("_ZN16EditorPauseLayerD0Ev", EditorPauseLayerHook::destructorH, EditorPauseLayerHook::destructor);
}