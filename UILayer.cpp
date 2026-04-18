#include "UILayer.hpp"
#include "PlayLayer.hpp"
#include "Setting.hpp"

void UILayer::Callback::onPrevStartPos(CCObject*) {
	PlayLayer::prevStartPos();
}

void UILayer::Callback::onNextStartPos(CCObject*) {
	PlayLayer::nextStartPos();
}

bool __fastcall UILayer::initH(gd::UILayer* self) {
	if (!UILayer::init(self)) return false;

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	auto startPosSwitcherMenu = CCMenu::create();
	self->addChild(startPosSwitcherMenu, 0, 125);

	auto startPosSwitcherLabel = CCLabelBMFont::create("", "bigFont.fnt");
	startPosSwitcherLabel->setPosition(startPosSwitcherMenu->convertToNodeSpace({ winSize.width / 2.f, director->getScreenBottom() + 20.f }));
	startPosSwitcherLabel->setScale(.5f);
	startPosSwitcherLabel->setOpacity(0);
	startPosSwitcherLabel->setVisible(false);
	startPosSwitcherMenu->addChild(startPosSwitcherLabel, 0, 1);

	auto onPrevStartPosSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
	onPrevStartPosSpr->setScale(.4f);
	auto onPrevStartPos = gd::CCMenuItemSpriteExtra::create(onPrevStartPosSpr, self, menu_selector(UILayer::Callback::onPrevStartPos));
	onPrevStartPos->setOpacity(0);
	onPrevStartPos->setVisible(false);
	onPrevStartPos->setPosition(startPosSwitcherMenu->convertToNodeSpace({ winSize.width / 2.f - 35.f, director->getScreenBottom() + 20.f }));
	startPosSwitcherMenu->addChild(onPrevStartPos, 0, 2);

	auto onNextStartPosSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
	onNextStartPosSpr->setScale(.4f);
	onNextStartPosSpr->setFlipX(true);
	auto onNextStartPos = gd::CCMenuItemSpriteExtra::create(onNextStartPosSpr, self, menu_selector(UILayer::Callback::onNextStartPos));
	onNextStartPos->setOpacity(0);
	onNextStartPos->setVisible(false);
	onNextStartPos->setPosition(startPosSwitcherMenu->convertToNodeSpace({ winSize.width / 2.f + 35.f, director->getScreenBottom() + 20.f }));
	startPosSwitcherMenu->addChild(onNextStartPos, 0, 3);

	return true;
}

void __fastcall UILayer::keyDownH(gd::UILayer* self, void*, enumKeyCodes key) {
	auto playLayer = gd::GameManager::sharedState()->getPlayLayer();

	if (key == setting().m_p1click) {
		if (playLayer) playLayer->pushButton(1, false);
	}
	else if (key == setting().m_p2click) {
		if (playLayer) playLayer->pushButton(1, true);
	}
	else {
		return UILayer::keyDown(self, key);
	}
}

void __fastcall UILayer::keyUpH(gd::UILayer* self, void*, enumKeyCodes key) {
	auto playLayer = gd::GameManager::sharedState()->getPlayLayer();

	if (key == setting().m_p1click) {
		if (playLayer) playLayer->releaseButton(1, false);
	}
	else if (key == setting().m_p2click) {
		if (playLayer) playLayer->releaseButton(1, true);
	}
	else {
		return UILayer::keyUp(self, key);
	}
}

void UILayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xfec60), UILayer::initH, reinterpret_cast<void**>(&UILayer::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xff130), UILayer::keyDownH, reinterpret_cast<void**>(&UILayer::keyDown));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xff2c0), UILayer::keyUpH, reinterpret_cast<void**>(&UILayer::keyUp));
}