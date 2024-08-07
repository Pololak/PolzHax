#include "MenuLayer.h"

bool isReload = false;

//void reload() {
//	isReload = true;
//
//	gd::GameManager::sharedState()->reloadAll(false, false, true);
//}

class ReloadTexAlertProtocol : public gd::FLAlertLayerProtocol {
protected:

	void FLAlert_Clicked(gd::FLAlertLayer* layer, bool btn2) override
	{
		if (btn2)
		{
			isReload = true;

			gd::GameManager::sharedState()->reloadAll(false, false, true);
		}
	}
};

ReloadTexAlertProtocol raProtocol;

void MenuLayer::Callback::onReload(CCObject*) {
	gd::FLAlertLayer::create(&raProtocol, "Reload Textures", "Are you sure you want to <cj>reload textures</c>?", "Cancel", "Yes", 300.f, false, 0)->show();
}

//void MenuLayer::Callback::onSmallEditorStep(CCObject*) {
//	gd::GameManager::sharedState()->toggleGameVariable("0035");
//}

static CCScene* scene(bool p0) {
	if (!isReload) return gd::MenuLayer::scene(p0);
	else {
		isReload = false;
		return gd::MenuLayer::scene(false);
	}
}

bool __fastcall MenuLayer::init_H(gd::MenuLayer* self, void* edx) {
	if (!MenuLayer::init(self)) return false;

	auto manager = gd::GameManager::sharedState();
	auto director = CCDirector::sharedDirector();
	auto menu = CCMenu::create();
	menu->setPosition(0, 0);

	auto reloadSpr = CCSprite::createWithSpriteFrameName("GJ_replayBtn_001.png");
	reloadSpr->setScale(0.525f);
	auto reloadBtn = gd::CCMenuItemSpriteExtra::create(reloadSpr, nullptr, self, menu_selector(MenuLayer::Callback::onReload));
	reloadBtn->setPosition({ director->getScreenLeft() + 18.750f, director->getScreenTop() - 55 });
	menu->addChild(reloadBtn);
	self->addChild(menu);

	/*auto smthSpr = CCSprite::createWithSpriteFrameName("GJ_playBtn_001.png");
	auto smthBtn = gd::CCMenuItemSpriteExtra::create(reloadSpr, nullptr, self, menu_selector(MenuLayer::Callback::onSmallEditorStep));
	menu->addChild(smthBtn);*/

	return true;
}

void MenuLayer::mem_init() {
	MH_CreateHook(
		reinterpret_cast<void*>(gd::base + 0xaf210),
		MenuLayer::init_H,
		reinterpret_cast<void**>(&MenuLayer::init));
}