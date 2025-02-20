#include "ObjectIDEnterPopup.h"

ObjectIDEnterPopup* ObjectIDEnterPopup::create(gd::EditorUI* edit) {
	ObjectIDEnterPopup* pRet = new ObjectIDEnterPopup();
	if (pRet && pRet->init(edit)) {
		pRet->autorelease();
		return pRet;
	}
	else {
		CC_SAFE_DELETE(pRet);
		return nullptr;
	}
}

bool ObjectIDEnterPopup::init(gd::EditorUI* editUI) {
	if (!this->initWithColor({ 0, 0, 0, 75 })) return false;

	m_mainLayer = CCLayer::create();
	this->addChild(m_mainLayer);

	m_buttonMenu = CCMenu::create();
	m_mainLayer->addChild(m_buttonMenu, 10);

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	auto bg = extension::CCScale9Sprite::create("GJ_square01.png", { 0.f, 0.f, 80.f, 80.f });
	bg->setContentSize({ 200.f, 160.f });
	bg->setPosition({ winSize / 2.f });
	m_mainLayer->addChild(bg);

	setTouchEnabled(true);
	setKeypadEnabled(true);

	return true;
}

void ObjectIDEnterPopup::keyBackClicked() {
	this->removeFromParentAndCleanup(true);
}