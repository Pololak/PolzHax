#include "SetGroupIDLayerHook.h"
#include "LevelEditorLayerHook.h"
#include "EditorUIHook.h"
#include "../utils.h"

class ObjectGroupInput : public cocos2d::CCLayer, TextInputDelegate {
protected:
	SetGroupIDLayer* m_parent = nullptr;

	virtual bool init(SetGroupIDLayer* parent) {
		m_parent = parent;
		if (!CCLayer::init()) return false;

		auto bg = extension::CCScale9Sprite::create("square02_small.png");
		bg->setContentSize({ 60.f, 35.f });
		bg->setOpacity(100);
		this->addChild(bg, -1);

		m_layerInput = CCTextInputNode::create(60.f, 35.f, "G", "bigFont.fnt");
		m_layerInput->setLabelPlaceholderColor(ccGRAY);
		m_layerInput->setAllowedChars("0123456789");
		m_layerInput->setDelegate(this);
		m_layerInput->setString(m_parent->m_groupLabel->getString());
		this->addChild(m_layerInput);

		return true;
	}

	virtual void textChanged(CCTextInputNode* input) {
		m_parent->m_groupIndex = std::atoi(input->getString().c_str());

		if (m_parent->m_targetObject == nullptr) {
            CCARRAY_FOREACH_B_TYPE(m_parent->m_targetObjects, object, GameObject) {
                if (object) {
					object->m_editorGroup = m_parent->m_groupIndex;
				}
            }
		}
		else {
			m_parent->m_targetObject->m_editorGroup = m_parent->m_groupIndex;
		}
	}

public:
	CCTextInputNode* m_layerInput = nullptr;

	static ObjectGroupInput* create(SetGroupIDLayer* parent) {
		ObjectGroupInput* ret = new ObjectGroupInput();
		if (ret && ret->init(parent)) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
};

void SetGroupIDLayerHook::Callback::onCurrentGroup(CCObject*) {
    auto editorLayer = LevelEditorLayerHook::get();

    if (editorLayer && editorLayer->m_groupIDFilter != -1) {
        this->m_groupIndex = editorLayer->m_groupIDFilter;
        this->updateLabel();
        this->updateGroupID();
    }
}

void SetGroupIDLayerHook::Callback::onNextFree(CCObject*) {
    auto editorLayer = LevelEditorLayerHook::get();

    if (editorLayer) {
        auto objs = editorLayer->getAllObjects();

        std::set<int> layers;

        CCARRAY_FOREACH_B_TYPE(objs, obj, GameObject) {
            layers.insert(obj->m_editorGroup);
        }

        int last = -1;
        for (auto const& layer : layers) {
            if (last + 1 != layer) break;
            last = layer;
        }

        this->m_groupIndex = last + 1;
        this->updateLabel();
        this->updateGroupID();
    }
}

bool SetGroupIDLayerHook::initH(SetGroupIDLayer* self, GameObject* object, CCArray* objects) {
    if (!SetGroupIDLayerHook::init(self, object, objects)) return false;

    self->m_buttonMenu->getChildByType<CCMenuItemSpriteExtra*>(1)->setSizeMult(1.2f);
    self->m_buttonMenu->getChildByType<CCMenuItemSpriteExtra*>(2)->setSizeMult(1.2f);

    self->m_groupLabel->setVisible(false);

    auto groupInput = ObjectGroupInput::create(self);
    groupInput->setPosition(self->m_groupLabel->getPosition());
    self->m_mainLayer->addChild(groupInput, 0, 127);

    auto onCurrentGroupSpr = ButtonSprite::create("Current Group", 90, 0, .5f, true, "bigFont.fnt", "GJ_button_04.png", 25.f);
    auto onCurrentGroup = CCMenuItemSpriteExtra::create(onCurrentGroupSpr, self, menu_selector(SetGroupIDLayerHook::Callback::onCurrentGroup));
    onCurrentGroup->setPositionX(-118.f);
    self->m_buttonMenu->addChild(onCurrentGroup);

    auto onNextFreeSpr = ButtonSprite::create("Next Free", 64, 0, .5f, true, "bigFont.fnt", "GJ_button_04.png", 25.f);
    auto onNextFree = CCMenuItemSpriteExtra::create(onNextFreeSpr, self, menu_selector(SetGroupIDLayerHook::Callback::onNextFree));
    onNextFree->setPositionX(130.f);
    self->m_buttonMenu->addChild(onNextFree);

    return true;
}

void SetGroupIDLayerHook::updateGroupIDH(SetGroupIDLayer* self) {
	SetGroupIDLayerHook::updateGroupID(self);

	auto groupInput = static_cast<ObjectGroupInput*>(self->m_mainLayer->getChildByTag(127));
	if (groupInput) {
		groupInput->m_layerInput->setString(self->m_groupLabel->getString());
	}

	if (EditorUIHook::get()) {
		EditorUIHook::updateObjectInfoLabel(EditorUIHook::get());
	}
}

void SetGroupIDLayerHook::mem_init() {
	HOOK("_ZN15SetGroupIDLayer4initEP10GameObjectPN7cocos2d7CCArrayE", SetGroupIDLayerHook::initH, SetGroupIDLayerHook::init);
	HOOK("_ZN15SetGroupIDLayer13updateGroupIDEv", SetGroupIDLayerHook::updateGroupIDH, SetGroupIDLayerHook::updateGroupID);
}