#include "MenuGameLayer.h"

bool __fastcall MenuGameLayer::hook(gd::MenuGameLayer* self) {
	gd::MenuGameLayer* MenuGameLayerObject = self;

	bool result = MenuGameLayer::init(self);
	auto director = CCDirector::sharedDirector();
	auto size = director->getWinSize();
	auto menu = CCMenu::create();

	auto WaveButton = CCSprite::createWithSpriteFrameName("gj_dartBtn_off_001.png");

	WaveButton->setPositionX(size.width / 3);
	WaveButton->setPositionY(size.height / 7);
	WaveButton->setRotation(-45);
	WaveButton->setZOrder(10);

	//self->addChild(WaveButton);
	//self->tryJump(123);

	return result;
}