#include "MenuLayer.h"

bool isReload = false;
CCLayer* testLayer;

//void reload() {
//	isReload = true;
//
//	gd::GameManager::sharedState()->reloadAll(false, false, true);
//}

class TestFLAlertProtocol : public CCLayer {
private:
	void cancelButtonCallback(CCObject*)
	{
		this->removeMeAndCleanup();
		testLayer = nullptr;
	}
public:
	virtual void keyBackClicked()
	{
		this->removeMeAndCleanup();
		testLayer = nullptr;
	}

	bool init() {
		CCLayer::init();
		CCLayerColor* cclcol = CCLayerColor::create(ccc4(0, 0, 0, 0));
		cclcol->setZOrder(1);
		cclcol->setScale(10.f);
		this->addChild(cclcol);
		auto actionCol = CCFadeTo::create(0.1f, 75);
		cclcol->runAction(actionCol);
		auto addingLayer = CCLayer::create();

		auto touchDispatcher = CCDirector::sharedDirector()->m_pTouchDispatcher;
		touchDispatcher->incrementForcePrio();
		touchDispatcher->incrementForcePrio();
		registerWithTouchDispatcher();
		setTouchEnabled(true);
		setKeypadEnabled(true);
		setMouseEnabled(true);

		auto bgSprite = CCSprite::create("GJ_button_03.png"); //Background button. Ik this is so bad, but i can't do anything better than that
		bgSprite->setScale(100.f);
		bgSprite->setOpacity(0);
		auto bgButton = gd::CCMenuItemSpriteExtra::create(bgSprite, nullptr, this, nullptr);
		auto bgMenu = CCMenu::create();
		bgMenu->addChild(bgButton);
		bgMenu->setZOrder(0);
		bgMenu->setPosition((CCDirector::sharedDirector()->getScreenRight()) - 25, (CCDirector::sharedDirector()->getScreenTop()) - 25);
		this->addChild(bgMenu);

		cocos2d::extension::CCScale9Sprite* bg = cocos2d::extension::CCScale9Sprite::create("GJ_square01.png");
		auto director = CCDirector::sharedDirector();
		bg->setContentSize({ 360, 180 });
		bg->setPosition((director->getScreenRight()) / 2, (director->getScreenTop()) / 2);
		bg->setZOrder(2);
		this->addChild(bg);
		auto appearAction = CCEaseBackOut::create(CCScaleTo::create(.25f, 1.f));

		auto closeMenu = CCMenu::create();
		closeMenu->setZOrder(3);

		auto closeSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
		auto closeBtn = gd::CCMenuItemSpriteExtra::create(closeSpr, nullptr, this, menu_selector(TestFLAlertProtocol::cancelButtonCallback));
		closeMenu->addChild(closeBtn);

		this->addChild(closeMenu);
		this->setScale(0.1f);
		this->runAction(appearAction);

		return true;
	}

	TestFLAlertProtocol* create() {
		TestFLAlertProtocol* obj = new TestFLAlertProtocol;
		if (obj->init()) obj->autorelease();
		else CC_SAFE_DELETE(obj);
		return obj;
	}

	void showCallback(CCObject* btn)
	{
		auto director = CCDirector::sharedDirector();
		auto testLayer = TestFLAlertProtocol::create();
		testLayer->setZOrder(100000);
		auto myLayer = static_cast<CCLayer*>(static_cast<CCNode*>(btn)->getUserObject());

		myLayer->addChild(testLayer);
		testLayer = testLayer;
	}
};

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

	auto somespr = CCSprite::create("GJ_button_01.png");
	auto somebtn = gd::CCMenuItemSpriteExtra::create(somespr, nullptr, self, menu_selector(TestFLAlertProtocol::showCallback));
	menu->addChild(somebtn);
	menu->addChild(reloadBtn);
	self->addChild(menu);

	return true;
}

void MenuLayer::mem_init() {
	MH_CreateHook(
		reinterpret_cast<void*>(gd::base + 0xaf210),
		MenuLayer::init_H,
		reinterpret_cast<void**>(&MenuLayer::init));
}