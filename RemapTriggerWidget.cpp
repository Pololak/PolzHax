#include "RemapTriggerWidget.hpp"
#include "LevelEditorLayer.hpp"

std::string RemapTriggerWidget::intToColorString(int id) {
	switch (id) {
	case 29: return "BG"; break;
	case 30: return "G"; break;
	case 104: return "Line"; break;
	case 105: return "Obj"; break;
	case 221: return "Col1"; break;
	case 717: return "Col2"; break;
	case 718: return "Col3"; break;
	case 743: return "Col4"; break;
	case 744: return "3DL"; break;
	default: return ""; break;
	}
}

void RemapTriggerWidget::setType(int id) {
	switch (id) {
	case 29: m_type = 0; break;
	case 30: m_type = 1; break;
	case 104: m_type = 2; break;
	case 105: m_type = 3; break;
	case 221: m_type = 4; break;
	case 717: m_type = 5; break;
	case 718: m_type = 6; break;
	case 743: m_type = 7; break;
	case 744: m_type = 8; break;
	}
}

const int m_triggerIds[] = { 29, 30, 104, 105, 221, 717, 718, 743, 744 };

bool RemapTriggerWidget::init(gd::ColorSelectPopup* parent) {
	m_parent = parent;
	this->setType(m_parent->m_targetObject->m_objectID);
	if (!CCLayer::init()) return false;

	auto labelBg = extension::CCScale9Sprite::create("square02_small.png");
	labelBg->setContentSize({ 40.f, 30.f });
	labelBg->setOpacity(100);
	this->addChild(labelBg);

	m_label = CCLabelBMFont::create("", "bigFont.fnt");
	this->addChild(m_label);

	this->updateLabel();

	return true;
}

void RemapTriggerWidget::updateLabel() {
	std::cout << m_type << std::endl;
	std::cout << m_triggerIds[m_type] << std::endl;

	if (m_label) {
		m_label->setString(this->intToColorString(m_parent->m_targetObject->m_objectID).c_str());
		m_label->setScale(1.f);
		m_label->limitLabelWidth(36.f, .8f, .1f);
	}
}

void RemapTriggerWidget::remapTrigger() {
	auto editorLayer = LevelEditorLayer::get();

	if (editorLayer) {
		auto object = m_parent->m_targetObject;
		LevelEditorLayer::removeTrigger(object);
		object->m_objectID = m_triggerIds[m_type];
		auto newFrameName = gd::ObjectToolbox::sharedState()->intKeyToFrame(object->m_objectID);
		if (newFrameName) {
			auto newSprite = CCSprite::createWithSpriteFrameName(newFrameName);
			if (newSprite) {
				object->m_textureName = newFrameName;
				object->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(newFrameName));
				object->setTextureRect(newSprite->getTextureRect());
			}
		}
		LevelEditorLayer::insertTrigger(object);

		m_parent->closeColorSelect(nullptr);
		editorLayer->m_uiLayer->editObject(nullptr);
	}
}

void RemapTriggerWidget::onTypeUp(CCObject*) {
	if (m_type < 8) {
		m_type++;
	}

	this->remapTrigger();
	this->updateLabel();
}

void RemapTriggerWidget::onTypeDown(CCObject*) {
	if (m_type > 0) {
		m_type--;
	}

	this->remapTrigger();
	this->updateLabel();
}