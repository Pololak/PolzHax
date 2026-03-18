#include "UILayer.hpp"
#include "Setting.hpp"

void UILayer::updateLabels() {
	if (!gd::GameManager::sharedState()->getPlayLayer()) return;
	if (!gd::GameManager::sharedState()->getPlayLayer()->m_uiLayer) return;

	auto playLayer = gd::GameManager::sharedState()->getPlayLayer();
	auto uiLayer = playLayer->m_uiLayer;

	if (CCLabelBMFont* messageLabel = static_cast<CCLabelBMFont*>(uiLayer->getChildByTag(502))) {
		messageLabel->setVisible(!setting().onHideLabels);
		messageLabel->setOpacity(setting().labelsOpacity * 255.f);
		messageLabel->setScale(setting().labelsScale);
		messageLabel->setString(setting().message.c_str());
	}
}

bool __fastcall UILayer::initH(gd::UILayer* self) {
	if (!UILayer::init(self)) return false;

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	/*auto messageLabel = CCLabelBMFont::create("", "bigFont.fnt");
	self->addChild(messageLabel, 105, 502);

	UILayer::updateLabels();*/

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