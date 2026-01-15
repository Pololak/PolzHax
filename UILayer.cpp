#include "UILayer.hpp"
#include "Setting.hpp"

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
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xff130), UILayer::keyDownH, reinterpret_cast<void**>(&UILayer::keyDown));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xff2c0), UILayer::keyUpH, reinterpret_cast<void**>(&UILayer::keyUp));
}