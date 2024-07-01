#include "MenuLayer.h"

CCLayer* menuLayerObject;
CCMenu* menuLayerBottomMenu = nullptr;

void MenuLayer::Callback::FLAlertBtn(CCObject*) {
	auto flalert = gd::FLAlertLayer::create(nullptr, "PolzHax Alpha", "<cr>IDI NAHUY</c> dolboyob. \nTOXIC: <cg>PIDOR</c>", "OK", nullptr, 250.0f, false, 0);
	flalert->show();
}

void MenuLayer::Callback::FLAlertGeode(CCObject*) {
	auto flalertgeode = gd::FLAlertLayer::create(nullptr, "ERROR", "Geode <cr>doesn't</c> support 1.92 version of <cg>Geometry Dash</c>", "OK :(", nullptr, 250.f, false, 0);
	flalertgeode->show();
}

void MenuLayer::Callback::zero(CCObject*) {
	
}
void MenuLayer::Callback::one(CCObject*) {

}
void MenuLayer::Callback::two(CCObject*) {

}

bool __fastcall MenuLayer::MenuLayer_init_H(CCLayer* self, void*) {
	menuLayerBottomMenu = nullptr;
	menuLayerObject = self;
	if (!MenuLayer::MenuLayer_init(self)) return false;

	auto btnArray = from<CCArray*>(menuLayerBottomMenu, 0xA8);
	auto achBtn = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(btnArray->objectAtIndex(0));
	auto optionsBtn = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(btnArray->objectAtIndex(1));
	auto statsBtn = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(btnArray->objectAtIndex(2));

	//menuLayerBottomMenu->setPositionX(menuLayerBottomMenu->getPositionX() - 27.5f);

	

	auto director = CCDirector::sharedDirector();
	auto size = director->getWinSize();
	auto menu = CCMenu::create();
	auto menu2 = CCMenu::create();
	auto labelm = CCLabelBMFont::create("PolzHax Alpha", "bigFont.fnt");

	auto zhigasprite = CCSprite::createWithSpriteFrameName("GJ_infoBtn_001.png");
	auto zhiga = gd::CCMenuItemSpriteExtra::create(zhigasprite, nullptr, self, menu_selector(MenuLayer::Callback::FLAlertBtn));

	auto geodeSpr = CCSprite::create("baseCircle_MediumAlt_Green.png");
	auto geodeBtn = gd::CCMenuItemSpriteExtra::create(geodeSpr, nullptr, self, menu_selector(MenuLayer::Callback::FLAlertGeode));
	geodeBtn->setPosition({ 112, -2 });

	auto smth = CCSprite::create("floor.png");
	smth->setPosition({ size.width / 2, size.height / 2 });
	//self->addChild(smth);

	auto selflbl = CCLabelBMFont::create(" ", "chatFont.fnt");
	selflbl->setPosition(100.f, 95.f);
	selflbl->setScale(0.66f);
	selflbl->setString(CCString::createWithFormat("MenuLayer: %p", self)->getCString());
	selflbl->setAnchorPoint({ 1.f, 0.5f });
	selflbl->setZOrder(100);
	//self->addChild(selflbl);

	auto menulbl = CCLabelBMFont::create(" ", "chatFont.fnt");
	menulbl->setPosition(280.f, 10.f);
	menulbl->setScale(0.66f);
	menulbl->setString(CCString::createWithFormat("Bottom Menu: % p", menuLayerBottomMenu)->getCString());
	menulbl->setAnchorPoint({ 1.f, 0.5f });
	menulbl->setZOrder(100);
	//self->addChild(menulbl);

	menu->setPosition({ size.width - 20, size.height - 20 });
	zhigasprite->setScale(0.8f);
	labelm->setScale(0.5f);
	labelm->setPosition(size.width / 2, size.height / 32);
	labelm->setOpacity(100);
	labelm->setZOrder(3);

	//menuLayerBottomMenu->addChild(geodeBtn);

	// testing switches

	auto labelzero = CCLabelBMFont::create("0", "bigFont.fnt");
	labelzero->setPositionY(-50);
	auto labelone = CCLabelBMFont::create("1", "bigFont.fnt");
	labelone->setPositionY(-50);
	labelone->setVisible(0);
	auto labeltwo = CCLabelBMFont::create("2", "bigFont.fnt");
	auto PlusSprite = CCSprite::createWithSpriteFrameName("GJ_zoomInBtn_001.png");
	auto MinusSprite = CCSprite::createWithSpriteFrameName("GJ_zoomOutBtn_001.png");

	if (&MenuLayer::Callback::one) {
		labelzero->setVisible(0);
		labelone->setVisible(1);
	}

	if (&MenuLayer::Callback::zero) {
		labelone->setVisible(0);
		labelzero->setVisible(1);
	}

	auto PlusBtn = gd::CCMenuItemSpriteExtra::create(PlusSprite, nullptr, self, menu_selector(MenuLayer::Callback::one));
	PlusBtn->setPosition({ 35,50 });
	auto MinusBtn = gd::CCMenuItemSpriteExtra::create(MinusSprite, nullptr, self, menu_selector(MenuLayer::Callback::zero));
	MinusBtn->setPosition({ -35,50 });

	auto testMenu = CCMenu::create();
	testMenu->addChild(PlusBtn);
	testMenu->addChild(MinusBtn);
	testMenu->addChild(labelzero);
	testMenu->addChild(labelone);
	self->addChild(testMenu);

	menu->addChild(zhiga);
	self->addChild(menu);
	
	return true;
}

void __fastcall MenuLayer::MenuLayer_bottomMenu_H() {
	_asm {
		mov menuLayerBottomMenu, eax
	}
	MenuLayer::MenuLayer_bottomMenu();
}