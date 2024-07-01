#include "GarageLayer.h"

bool __fastcall GarageLayer::hook(CCLayer* self) {
	CCLayer* GarageLayerObject = self;

	bool result = GarageLayer::init(self);
	auto director = CCDirector::sharedDirector();
	auto size = director->getWinSize();
	auto menu = CCMenu::create();

	auto scrnright = director->getScreenRight();
	auto scrnup = director->getScreenTop();

	auto zhigasprite = CCSprite::createWithSpriteFrameName("GJ_reportBtn_001.png");
	auto zhiga = gd::CCMenuItemSpriteExtra::create(zhigasprite, nullptr, self, menu_selector(gd::MenuLayer::onGarage));

	auto WaveButton = CCSprite::createWithSpriteFrameName("gj_dartBtn_off_001.png");

	menu->setPosition({ size.width - 150, size.height - 60 });
	zhigasprite->setScale(0.5f);


	//self->addChild(WaveButton);
	//menu->addChild(zhiga);
	//self->addChild(menu);

	return result;
}