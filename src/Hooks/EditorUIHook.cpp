#include "EditorUIHook.h"
#include "LevelEditorLayerHook.h"
#include "../GlobalClipboard.h"
#include "../GameVariables.h"
#include "../utils.h"
#include "../Setting.h"
#include "../RotateSaws.h"
#include "../Layers/NewCustomizeObjectLayer.h"

EditorUI* m_editorUI;

EditorUI* EditorUIHook::get() {
    return m_editorUI;
}

void EditorUIHook::updateObjectInfoLabel(EditorUI* self) {
    if (!GameManager::sharedState()->getGameVariable(SHOW_OBJECT_INFO));

    auto objectInfoLabel = static_cast<CCLabelBMFont*>(self->getChildByTag(2701));
    if (objectInfoLabel) {
        std::stringstream ss;

        if (self->m_selectedObject || self->m_selectedObjects->count() == 1) {
            GameObject* object = self->m_selectedObject;
            if (!self->m_selectedObject && self->m_selectedObjects->count() == 1) {
                object = reinterpret_cast<GameObject*>(self->m_selectedObjects->objectAtIndex(0));
            }

            ss << "C: " << colorToString(static_cast<int>(object->getColorMode())) << " (" << static_cast<int>(object->getColorMode()) << ")" << "\n";
            ss << "G: " << object->m_editorGroup << "\n";
            ss << "Rot: " << object->getRotation() << "\n";
            ss << "X: " << object->getPositionX() << "\n";
            ss << "Y: " << object->getPositionY() << "\n";
            ss << "ID: " << object->m_objectID << "\n";
            ss << "Type: " << typeToString(object->m_objectType) << "\n";
            ss << "Time: " << self->m_editorLayer->m_gridLayer->timeForXPos(object->getPositionX()) << "\n";
            ss << "Addr: 0x" << std::hex << reinterpret_cast<uintptr_t>(object) << std::dec << "\n";
        }
        else if (self->m_selectedObjects->count() > 1) {
            ss << "Objects: " << self->m_selectedObjects->count() << "\n";
        }
        else {
            ss.clear();
        }

        objectInfoLabel->setString(ss.str().c_str());
    }
}

void EditorUIHook::updateGuideTogglePosition(EditorUI* self) {
	auto onAllGroup = static_cast<CCMenuItemSpriteExtra*>(static_cast<CCMenu*>(self->m_deselectBtn->getParent())->getChildByTag(2702));
	if (onAllGroup) {
		self->m_guideToggle->setPositionX(onAllGroup->isVisible() ? (self->m_groupPrevBtn->getPositionX() - 46.f) : (self->m_groupPrevBtn->getPositionX() - 26.f));
	}
}

void EditorUIHook::Callback::onAllGroup(CCObject* sender) {
    this->m_currentGroupLabel->setString("All");
    this->m_editorLayer->m_groupIDFilter = -1;

    static_cast<CCMenuItemSpriteExtra*>(sender)->setEnabled(false);
    static_cast<CCMenuItemSpriteExtra*>(sender)->setVisible(false);

    EditorUIHook::updateGuideTogglePosition(this);

    
}

void EditorUIHook::Callback::onNextFreeGroup(CCObject*) {
    auto objs = this->m_editorLayer->getAllObjects();

    std::set<int> layers;

    CCARRAY_FOREACH_B_TYPE(objs, obj, GameObject) {
        layers.insert(obj->m_editorGroup);
    }

    int last = -1;
    for (auto const& layer : layers) {
        if (last + 1 != layer) break;
        last = layer;
    }

    this->m_currentGroupLabel->setString(CCString::createWithFormat("%d", last + 1)->getCString());
    this->m_editorLayer->m_groupIDFilter = last + 1;

    auto onAllGroup = static_cast<CCMenuItemSpriteExtra*>(static_cast<CCMenu*>(this->m_deselectBtn->getParent())->getChildByTag(2702));
    if (onAllGroup) {
        onAllGroup->setVisible(true);
        onAllGroup->setEnabled(true);
    }

    EditorUIHook::updateGuideTogglePosition(this);


}

void EditorUIHook::Callback::onGoToGroup(CCObject*) {
    auto objs = this->getSelectedObjects();

    CCARRAY_FOREACH_B_TYPE(objs, obj, GameObject) {
        if (obj) {
            int objectGroup = obj->m_editorGroup;
            this->m_editorLayer->m_groupIDFilter = objectGroup;
            this->m_currentGroupLabel->setString(CCString::createWithFormat("%d", objectGroup)->getCString());

            auto onAllGroup = static_cast<CCMenuItemSpriteExtra*>(static_cast<CCMenu*>(this->m_deselectBtn->getParent())->getChildByTag(2702));
            if (onAllGroup) {
                onAllGroup->setVisible(true);
                onAllGroup->setEnabled(true);
            }

            EditorUIHook::updateGuideTogglePosition(this);


        }
    }
}

bool EditorUIHook::initH(EditorUI* self, LevelEditorLayer* editorLayer) {
    m_editorUI = self;
    if (!EditorUIHook::init(self, editorLayer)) return false;

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    CCMenu* leftMenu = static_cast<CCMenu*>(self->m_undoBtn->getParent());
    CCMenu* rightMenu = static_cast<CCMenu*>(self->m_deselectBtn->getParent());

    self->m_positionSlider->setAnchorPoint({ 0.f, 0.f });
    self->m_positionSlider->setScale(.8f);

    auto objectInfoLabel = CCLabelBMFont::create("", "chatFont.fnt");
    objectInfoLabel->setAnchorPoint({ 0.f, 1.f });
    objectInfoLabel->setScale(.6f);
    objectInfoLabel->setPosition(director->getScreenLeft() + 50.f, director->getScreenTop() - 50.f);
    objectInfoLabel->setVisible(GameManager::sharedState()->getGameVariable(SHOW_OBJECT_INFO));
    self->addChild(objectInfoLabel, 0, 2701);

    auto onTrashSpr = CCSprite::create("GJ_trashBtn_001.png");
    onTrashSpr->setScale(.925f);
    auto onTrash = CCMenuItemSpriteExtra::create(onTrashSpr, self, menu_selector(EditorUI::onDeleteSelected));
    onTrash->setPosition(self->m_redoBtn->getPositionX() + 50.f, self->m_redoBtn->getPositionY() - 1.f);
    onTrash->setOpacity(175);
    onTrash->setColor(ccGRAY);
    onTrash->setEnabled(false);
    leftMenu->addChild(onTrash, 0, 201);

    self->m_groupPrevBtn->setPositionX(self->m_groupPrevBtn->getPositionX() - 10.f);
    self->m_groupNextBtn->setPositionX(self->m_groupNextBtn->getPositionX() - 10.f);
    self->m_currentGroupLabel->setPositionX(self->m_currentGroupLabel->getPositionX() - 10.f);

    auto onAllGroupSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    onAllGroupSpr->setScale(.5f);
    onAllGroupSpr->setOpacity(175);
    auto onAllGroup = CCMenuItemSpriteExtra::create(onAllGroupSpr, self, menu_selector(EditorUIHook::Callback::onAllGroup));
    onAllGroup->setPosition(-90.f, -172.f);
    rightMenu->addChild(onAllGroup, 0, 2702);
    onAllGroup->setVisible(!(self->m_editorLayer->m_groupIDFilter == -1));
    onAllGroup->setEnabled(!(self->m_editorLayer->m_groupIDFilter == -1));

    auto onNextFreeGroupSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    onNextFreeGroupSpr->setFlipX(true);
    onNextFreeGroupSpr->setScale(.5f);
    onNextFreeGroupSpr->setOpacity(175);
    auto onNextFreeGroup = CCMenuItemSpriteExtra::create(onNextFreeGroupSpr, self, menu_selector(EditorUIHook::Callback::onNextFreeGroup));
    onNextFreeGroup->setPosition(10.f, -172.f);
    rightMenu->addChild(onNextFreeGroup);

    EditorUIHook::updateGuideTogglePosition(self);

    auto onGoToGroupSpr = CCSprite::create("GJ_goToGroupBtn_001.png");
    onGoToGroupSpr->setScale(.85f);
    onGoToGroupSpr->setPositionY(onGoToGroupSpr->getPositionY() - 1.f);
    auto onGoToGroup = CCMenuItemSpriteExtra::create(onGoToGroupSpr, self, menu_selector(EditorUIHook::Callback::onGoToGroup));
    onGoToGroup->setPosition(self->m_editGroupBtn->getPositionX() - 44.f, self->m_editGroupBtn->getPositionY());
    onGoToGroup->setOpacity(175);
    onGoToGroup->setColor(ccGRAY);
    onGoToGroup->setEnabled(false);
    onGoToGroup->setVisible(false);
    rightMenu->addChild(onGoToGroup, 0, 2704);

    // // fake buttons lol
    // auto buttonPageMenu = static_cast<CCMenu*>(static_cast<ButtonPage*>(self->m_editButtonBar->m_pagesArray->objectAtIndex(1))->getChildren()->objectAtIndex(0));

    // if (buttonPageMenu) {
    //     auto freeRotateBtn = static_cast<CCMenuItemSpriteExtra*>(self->m_editButtonDict->objectForKey("21"));
    //     auto snapRotateBtn = static_cast<CCMenuItemSpriteExtra*>(self->m_editButtonDict->objectForKey("22"));

    //     auto fakeFreeRotate = self->getSpriteButton("edit_freeRotateBtn_001.png", nullptr, nullptr, .9f);
    //     auto fakeSnapRotate = self->getSpriteButton("edit_rotateSnapBtn_001.png", nullptr, nullptr, .9f);
        
    //     fakeFreeRotate->setEnabled(false);
    //     static_cast<ButtonSprite*>(fakeFreeRotate->getChildren()->objectAtIndex(0))->setColor(ccGRAY);
    //     fakeSnapRotate->setEnabled(false);
    //     static_cast<ButtonSprite*>(fakeSnapRotate->getChildren()->objectAtIndex(0))->setColor(ccGRAY);

    //     fakeFreeRotate->setPosition(freeRotateBtn->getPosition());
    //     fakeSnapRotate->setPosition(snapRotateBtn->getPosition());

    //     buttonPageMenu->addChild(fakeFreeRotate, -1);
    //     buttonPageMenu->addChild(fakeSnapRotate, -1);
    // }
    // //

    return true;
}

void EditorUIHook::selectObjectH(EditorUI* self, GameObject* object) {
    EditorUIHook::updateObjectInfoLabel(self);

    EditorUIHook::selectObject(self, object);
}

void EditorUIHook::selectObjectsH(EditorUI* self, CCArray* objects) {
    EditorUIHook::updateObjectInfoLabel(self);

    EditorUIHook::selectObjects(self, objects);
}

void EditorUIHook::moveObjectH(EditorUI* self, GameObject* object, CCPoint pos) {
    if (object == nullptr) return;

    EditorUIHook::moveObject(self, object, pos);

    EditorUIHook::updateObjectInfoLabel(self);

    LevelEditorLayerHook::moveTrigger(object);
}

void EditorUIHook::angleChangedH(EditorUI* self, float angle) {
    EditorUIHook::angleChanged(self, angle);

    EditorUIHook::updateObjectInfoLabel(self);
}

void EditorUIHook::updateButtonsH(EditorUI* self) {
    EditorUIHook::updateButtons(self);

    CCMenu* leftMenu = static_cast<CCMenu*>(self->m_undoBtn->getParent());
    CCMenu* rightMenu = static_cast<CCMenu*>(self->m_deselectBtn->getParent());

    auto onTrash = static_cast<CCMenuItemSpriteExtra*>(leftMenu->getChildByTag(201));
    if (onTrash) {
        onTrash->setOpacity(self->getSelectedObjects()->count() ? 255 : 175);
        onTrash->setColor(self->getSelectedObjects()->count() ? ccWHITE : ccGRAY);
        onTrash->setEnabled(self->getSelectedObjects()->count());
    }

    auto onGoToGroup = static_cast<CCMenuItemSpriteExtra*>(rightMenu->getChildByTag(2704));
    if (onGoToGroup) {
        onGoToGroup->setOpacity(self->getSelectedObjects()->count() ? 255 : 175);
        onGoToGroup->setColor(self->getSelectedObjects()->count() ? ccWHITE : ccGRAY);
        onGoToGroup->setEnabled(self->getSelectedObjects()->count());
        onGoToGroup->setVisible(self->getSelectedObjects()->count());
    }

    EditorUIHook::updateObjectInfoLabel(self);
}

void EditorUIHook::clickOnPositionH(EditorUI* self, CCPoint pos) {
    EditorUIHook::clickOnPosition(self, pos);

    EditorUIHook::updateObjectInfoLabel(self);
}

CCPoint EditorUIHook::offsetForKeyH(EditorUI* self, int id) {
    switch (id) {
    case 194: return ccp(-4.5f, 4.5f); break;
    case 406: return ccp(0.f, -8.f); break;
    case 407: return ccp(0.f, -11.f); break;
    case 408: return ccp(0.f, -12.5f); break;
    case 419: return ccp(0.f, -2.5f); break;
    case 420: return ccp(0.f, -2.5f); break;
    case 421: return ccp(0.f, -9.f); break;
    case 422: return ccp(0.f, -9.f); break;
    default: return EditorUIHook::offsetForKey(self, id); break;
    }
}

CCPoint EditorUIHook::moveForCommandH(EditorUI* self, EditCommand command) {
	float gridSize = self->m_gridSize;

	switch (command) {
	case static_cast<EditCommand>(101): return ccp(-1.f / 2.f, 0.f) * gridSize;
	case static_cast<EditCommand>(102): return ccp(1.f / 2.f, 0.f) * gridSize;
	case static_cast<EditCommand>(103): return ccp(0.f, 1.f / 2.f) * gridSize;
	case static_cast<EditCommand>(104): return ccp(0.f, -1.f / 2.f) * gridSize;

	case static_cast<EditCommand>(105): return ccp(-1.f / 4.f, 0.f) * gridSize;
	case static_cast<EditCommand>(106): return ccp(1.f / 4.f, 0.f) * gridSize;
	case static_cast<EditCommand>(107): return ccp(0.f, 1.f / 4.f) * gridSize;
	case static_cast<EditCommand>(108): return ccp(0.f, -1.f / 4.f) * gridSize;

	case static_cast<EditCommand>(109): return ccp(-1.f / 8.f, 0.f) * gridSize;
	case static_cast<EditCommand>(110): return ccp(1.f / 8.f, 0.f) * gridSize;
	case static_cast<EditCommand>(111): return ccp(0.f, 1.f / 8.f) * gridSize;
	case static_cast<EditCommand>(112): return ccp(0.f, -1.f / 8.f) * gridSize;

	case static_cast<EditCommand>(113): return ccp(-.5f, 0.f);
	case static_cast<EditCommand>(114): return ccp(.5f, 0.f);
	case static_cast<EditCommand>(115): return ccp(0.f, .5f);
	case static_cast<EditCommand>(116): return ccp(0.f, -.5f);

	case static_cast<EditCommand>(117): return ccp(-.1f, 0.f);
	case static_cast<EditCommand>(118): return ccp(.1f, 0.f);
	case static_cast<EditCommand>(119): return ccp(0.f, .1f);
	case static_cast<EditCommand>(120): return ccp(0.f, -.1f);

	default: return EditorUIHook::moveForCommand(self, command);
	}
}

void EditorUIHook::transformObjectH(EditorUI* self, GameObject* object, EditCommand command, bool p0) {
    CCArray* selectedObjects = self->getSelectedObjects();
    int selectedObjectsCount = selectedObjects->count();

    if (object->canRotateFree()) {
        switch (command) {
        case static_cast<EditCommand>(121):
            self->rotateObjects(selectedObjects, (45.f / selectedObjectsCount), ccp(0.f, 0.f)); break;
        case static_cast<EditCommand>(122):
            self->rotateObjects(selectedObjects, -(45.f / selectedObjectsCount), ccp(0.f, 0.f)); break;
        case static_cast<EditCommand>(123):
            self->rotateObjects(selectedObjects, (26.f / selectedObjectsCount), ccp(0.f, 0.f)); break;
        case static_cast<EditCommand>(124):
            self->rotateObjects(selectedObjects, -(26.f / selectedObjectsCount), ccp(0.f, 0.f)); break;
        }
    }

    EditorUIHook::transformObject(self, object, command, p0);

    EditorUIHook::updateObjectInfoLabel(self);
}

void EditorUIHook::onCopyH(EditorUI* self, CCObject* sender) {
    if (setting().onPreviewRotations) RotateSaws::stopRotations(self->m_editorLayer);

    EditorUIHook::onCopy(self, sender);

    if (setting().onPreviewRotations) RotateSaws::beginRotations(self->m_editorLayer);
}

void EditorUIHook::onGroupDownH(EditorUI* self, CCObject* sender) {
    EditorUIHook::onGroupDown(self, sender);
    auto onAllGroup = static_cast<CCMenuItemSpriteExtra*>(static_cast<CCMenu*>(self->m_deselectBtn->getParent())->getChildByTag(2702));
    if (onAllGroup) {
        onAllGroup->setVisible(!(self->m_editorLayer->m_groupIDFilter == -1));
        onAllGroup->setEnabled(!(self->m_editorLayer->m_groupIDFilter == -1));
    }

    EditorUIHook::updateGuideTogglePosition(self);
}

void EditorUIHook::onGroupUpH(EditorUI* self, CCObject* sender) {
    EditorUIHook::onGroupUp(self, sender);
    auto onAllGroup = static_cast<CCMenuItemSpriteExtra*>(static_cast<CCMenu*>(self->m_deselectBtn->getParent())->getChildByTag(2702));
    if (onAllGroup) {
        onAllGroup->setVisible(true);
        onAllGroup->setEnabled(true);
    }

    EditorUIHook::updateGuideTogglePosition(self);
}

void EditorUIHook::onDuplicateH(EditorUI* self, CCObject* sender) {
    // Wacky bug fix
    int currentEditorLayer = self->m_editorLayer->m_groupIDFilter;

    self->m_editorLayer->m_groupIDFilter = -1;

    if (setting().onPreviewRotations) RotateSaws::stopRotations(self->m_editorLayer);

    EditorUIHook::onDuplicate(self, sender);

    if (setting().onPreviewRotations) RotateSaws::beginRotations(self->m_editorLayer);

    self->m_editorLayer->m_groupIDFilter = currentEditorLayer;
}

void EditorUIHook::editObjectH(EditorUI* self, CCObject* sender) {
	if (setting().onNewColorSelectMenu) {
		if (self->editButtonUsable()) {
			if ((self->m_selectedObject == nullptr) || (self->m_selectedObject->m_objectType != GameObjectType::SecretCoin)) {
				if ((self->m_selectedObject == nullptr) || (self->m_selectedObject->m_objectID != 31)) {
					if ((self->m_selectedObject == nullptr) || self->m_selectedObject->canChangeCustomColor()) {
						if ((self->m_selectedObjects->count() != 0) || self->m_selectedObject != nullptr && self->m_selectedObject->canChangeCustomColor()) {
							NewCustomizeObjectLayer::create(self->m_selectedObject, self->m_selectedObjects)->show();
							return;
						}
					}
				}
			}
		}
	}
	
	EditorUIHook::editObject(self, sender);
}

void EditorUIHook::constrainGameLayerPositionH(EditorUI* self, float p0, float p1) {
    if (!setting().onFreeScroll) EditorUIHook::constrainGameLayerPosition(self, p0, p1);
}

void EditorUIHook::destructorH(EditorUI* self) {
    EditorUIHook::destructor(self);
    m_editorUI = nullptr;
}

void EditorUIHook::mem_init() {
    HOOK("_ZN8EditorUI4initEP16LevelEditorLayer", EditorUIHook::initH, EditorUIHook::init);
    HOOK("_ZN8EditorUI12selectObjectEP10GameObject", EditorUIHook::selectObjectH, EditorUIHook::selectObject);
    HOOK("_ZN8EditorUI13selectObjectsEPN7cocos2d7CCArrayE", EditorUIHook::selectObjectsH, EditorUIHook::selectObjects);
    HOOK("_ZN8EditorUI10moveObjectEP10GameObjectN7cocos2d7CCPointE", EditorUIHook::moveObjectH, EditorUIHook::moveObject);
    HOOK("_ZN8EditorUI12angleChangedEf", EditorUIHook::angleChangedH, EditorUIHook::angleChanged);
    HOOK("_ZN8EditorUI13updateButtonsEv", EditorUIHook::updateButtonsH, EditorUIHook::updateButtons);
    HOOK("_ZN8EditorUI15clickOnPositionEN7cocos2d7CCPointE", EditorUIHook::clickOnPositionH, EditorUIHook::clickOnPosition);
    HOOK("_ZN8EditorUI12offsetForKeyEi", EditorUIHook::offsetForKeyH, EditorUIHook::offsetForKey);
    HOOK("_ZN8EditorUI14moveForCommandE11EditCommand", EditorUIHook::moveForCommandH, EditorUIHook::moveForCommand);
    HOOK("_ZN8EditorUI15transformObjectEP10GameObject11EditCommandb", EditorUIHook::transformObjectH, EditorUIHook::transformObject);
    HOOK("_ZN8EditorUI6onCopyEPN7cocos2d8CCObjectE", EditorUIHook::onCopyH, EditorUIHook::onCopy);
    HOOK("_ZN8EditorUI11onGroupDownEPN7cocos2d8CCObjectE", EditorUIHook::onGroupDownH, EditorUIHook::onGroupDown);
    HOOK("_ZN8EditorUI9onGroupUpEPN7cocos2d8CCObjectE", EditorUIHook::onGroupUpH, EditorUIHook::onGroupUp);
    HOOK("_ZN8EditorUI11onDuplicateEPN7cocos2d8CCObjectE", EditorUIHook::onDuplicateH, EditorUIHook::onDuplicate);
    HOOK("_ZN8EditorUI10editObjectEPN7cocos2d8CCObjectE", EditorUIHook::editObjectH, EditorUIHook::editObject);
    HOOK("_ZN8EditorUI26constrainGameLayerPositionEff", EditorUIHook::constrainGameLayerPositionH, EditorUIHook::constrainGameLayerPosition);

    HOOK("_ZN8EditorUID0Ev", EditorUIHook::destructorH, EditorUIHook::destructor);
}