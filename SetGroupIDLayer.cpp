#include "SetGroupIDLayer.hpp"
#include "EditorUI.hpp"
#include "utils.hpp"

class ObjectGroupInput : public cocos2d::CCLayer, gd::TextInputDelegate {
protected:
	gd::SetGroupIDLayer* m_parent = nullptr;

	virtual bool init(gd::SetGroupIDLayer* parent) {
		m_parent = parent;
		if (!CCLayer::init()) return false;

		auto bg = extension::CCScale9Sprite::create("square02_small.png");
		bg->setContentSize({ 60.f, 35.f });
		bg->setOpacity(100);
		this->addChild(bg, -1);

		m_layerInput = gd::CCTextInputNode::create(60.f, 35.f, "G", this, "bigFont.fnt");
		m_layerInput->setLabelPlaceholderColor(ccGRAY);
		m_layerInput->setAllowedChars("0123456789");
		m_layerInput->setDelegate(this);
		m_layerInput->setString(m_parent->m_groupLabel->getString());
		this->addChild(m_layerInput);

		return true;
	}

	virtual void textChanged(gd::CCTextInputNode* input) {
		m_parent->m_groupIndex = std::atoi(input->getString().c_str());

		if (m_parent->m_targetObject == nullptr) {
			for (int i = 0; i < m_parent->m_targetObjects->count(); i++) {
				auto object = reinterpret_cast<gd::GameObject*>(m_parent->m_targetObjects->objectAtIndex(i));
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
	gd::CCTextInputNode* m_layerInput = nullptr;

	static ObjectGroupInput* create(gd::SetGroupIDLayer* parent) {
		ObjectGroupInput* ret = new ObjectGroupInput();
		if (ret && ret->init(parent)) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
};

void SetGroupIDLayer::Callback::onCurrentGroup(CCObject*) {
	auto editorLayer = EditorUI::get()->m_editorLayer;

	if (editorLayer->m_groupIDFilter != -1) {
		this->m_groupIndex = editorLayer->m_groupIDFilter;
		this->updateLabel();
		this->updateGroupID();
	}
}

void SetGroupIDLayer::Callback::onNextFree(CCObject*) {
	auto objs = EditorUI::get()->m_editorLayer->getAllObjects();

	std::set<int> layers;

	CCARRAY_FOREACH_B_TYPE(objs, obj, gd::GameObject) {
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

bool __fastcall SetGroupIDLayer::initH(gd::SetGroupIDLayer* self, void*, gd::GameObject* object, CCArray* objects) {
	if (!SetGroupIDLayer::init(self, object, objects)) return false;

	reinterpret_cast<gd::CCMenuItemSpriteExtra*>(self->m_buttonMenu->getChildren()->objectAtIndex(1))->setSizeMult(1.f);
	reinterpret_cast<gd::CCMenuItemSpriteExtra*>(self->m_buttonMenu->getChildren()->objectAtIndex(2))->setSizeMult(1.f);

	self->m_groupLabel->setVisible(false);

	auto groupInput = ObjectGroupInput::create(self);
	groupInput->setPosition(self->m_groupLabel->getPosition());
	self->m_mainLayer->addChild(groupInput, 0, 127);

	auto onCurrentGroupSpr = gd::ButtonSprite::create("Current Group", 90, 0, .5f, true, "bigFont.fnt", "GJ_button_04.png", 25.f);
	auto onCurrentGroup = gd::CCMenuItemSpriteExtra::create(onCurrentGroupSpr, self, menu_selector(SetGroupIDLayer::Callback::onCurrentGroup));
	onCurrentGroup->setPositionX(-118.f);
	self->m_buttonMenu->addChild(onCurrentGroup);

	auto onNextFreeSpr = gd::ButtonSprite::create("Next Free", 64, 0, .5f, true, "bigFont.fnt", "GJ_button_04.png", 25.f);
	auto onNextFree = gd::CCMenuItemSpriteExtra::create(onNextFreeSpr, self, menu_selector(SetGroupIDLayer::Callback::onNextFree));
	onNextFree->setPositionX(130.f);
	self->m_buttonMenu->addChild(onNextFree);

	return true;
}

void __fastcall SetGroupIDLayer::updateGroupIDH(gd::SetGroupIDLayer* self) {
	SetGroupIDLayer::updateGroupID(self);

	auto groupInput = static_cast<ObjectGroupInput*>(self->m_mainLayer->getChildByTag(127));
	if (groupInput) {
		groupInput->m_layerInput->setString(self->m_groupLabel->getString());
	}
}

void __fastcall SetGroupIDLayer::destructorH(gd::SetGroupIDLayer* self) {
	SetGroupIDLayer::destructor(self);

	if (EditorUI::get()) {
		EditorUI::updateObjectInfoLabel(EditorUI::get());
	}
}

void SetGroupIDLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xf78d0), SetGroupIDLayer::initH, reinterpret_cast<void**>(&SetGroupIDLayer::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xf7e80), SetGroupIDLayer::updateGroupIDH, reinterpret_cast<void**>(&SetGroupIDLayer::updateGroupID));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xf7760), SetGroupIDLayer::destructorH, reinterpret_cast<void**>(&SetGroupIDLayer::destructor));
}